#include <config.h>
#include <string.h>

#include "libgnomeP.h"

/* Name of config key we use when looking up preferred language. */
#define LANGKEY "/Gnome/i18n/LANG"

static GHashTable *alias_table = NULL;

/*read an alias file for the locales*/
static void
read_aliases (char *file)
{
  FILE *fp;
  char buf[256];
  if (!alias_table)
    alias_table = g_hash_table_new (g_str_hash, g_str_equal);
  fp = fopen (file,"r");
  if (!fp)
    return;
  while (fgets (buf,sizeof (buf),fp))
    {
      char *p;
      g_strstrip(buf);
      if (buf[0]=='#' || buf[0]=='\0')
        continue;
      p = strtok (buf,"\t ");
      if (!p)
	continue;
      p = strtok (NULL,"\t ");
      if(!p)
	continue;
      if (!g_hash_table_lookup (alias_table, buf))
	g_hash_table_insert (alias_table, g_strdup(buf), g_strdup(p));
    }
  fclose (fp);
}

/*return the un-aliased language as a newly allocated string*/
static char *
unalias_lang (char *lang)
{
  char *p;
  if (!alias_table)
    {
      read_aliases ("/usr/share/locale/locale.alias");
      read_aliases ("/usr/local/share/locale/locale.alias");
      read_aliases ("/usr/lib/X11/locale/locale.alias");
      read_aliases ("/usr/openwin/lib/locale/locale.alias");
    }
  while ((p=g_hash_table_lookup(alias_table,lang)) && strcmp(p, lang))
      lang = p;
  return lang;
}

/**
 * gnome_i18n_get_language:
 * 
 * Returns current language (contents of "LANG" environment variable).
 * 
 * Return value: 
 **/
const char *
gnome_i18n_get_language(void)
{
  return getenv("LANG");
}

/* Mask for components of locale spec. The ordering here is from
 * least significant to most significant
 */
enum
{
  COMPONENT_CODESET =   1 << 0,
  COMPONENT_TERRITORY = 1 << 1,
  COMPONENT_MODIFIER =  1 << 2
};

/* Break an X/Open style locale specification into components
 */
static guint
explode_locale (const gchar *locale,
		gchar **language, 
		gchar **territory, 
		gchar **codeset, 
		gchar **modifier)
{
  const gchar *uscore_pos;
  const gchar *at_pos;
  const gchar *dot_pos;

  guint mask = 0;

  uscore_pos = strchr (locale, '_');
  dot_pos = strchr (uscore_pos ? uscore_pos : locale, '.');
  at_pos = strchr (dot_pos ? dot_pos : (uscore_pos ? uscore_pos : locale), '@');

  if (at_pos)
    {
      mask |= COMPONENT_MODIFIER;
      *modifier = g_strdup (at_pos);
    }
  else
    at_pos = locale + strlen (locale);

  if (dot_pos)
    {
      mask |= COMPONENT_CODESET;
      *codeset = g_new (gchar, 1 + at_pos - dot_pos);
      strncpy (*codeset, dot_pos, at_pos - dot_pos);
      (*codeset)[at_pos - dot_pos] = '\0';
    }
  else
    dot_pos = at_pos;

  if (uscore_pos)
    {
      mask |= COMPONENT_TERRITORY;
      *territory = g_new (gchar, 1 + dot_pos - uscore_pos);
      strncpy (*territory, uscore_pos, dot_pos - uscore_pos);
      (*territory)[dot_pos - uscore_pos] = '\0';
    }
  else
    uscore_pos = dot_pos;

  *language = g_new (gchar, 1 + uscore_pos - locale);
  strncpy (*language, locale, uscore_pos - locale);
  (*language)[uscore_pos - locale] = '\0';

  return mask;
}

/*
 * Compute all interesting variants for a given locale name -
 * by stripping off different components of the value.
 *
 * For simplicity, we assume that the locale is in
 * X/Open format: language[_territory][.codeset][@modifier]
 *
 * TODO: Extend this to handle the CEN format (see the GNUlibc docs)
 *       as well. We could just copy the code from glibc wholesale
 *       but it is big, ugly, and complicated, so I'm reluctant
 *       to do so when this should handle 99% of the time...
 */
static GList *
compute_locale_variants (const gchar *locale)
{
  GList *retval = NULL;

  gchar *language;
  gchar *territory;
  gchar *codeset;
  gchar *modifier;

  guint mask;
  guint i;

  g_return_val_if_fail (locale != NULL, NULL);

  mask = explode_locale (locale, &language, &territory, &codeset, &modifier);

  /* Iterate through all possible combinations, from least attractive
   * to most attractive.
   */
  for (i=0; i<=mask; i++)
    if ((i & ~mask) == 0)
      {
	gchar *val = g_strconcat(language,
				 (i & COMPONENT_TERRITORY) ? territory : "",
				 (i & COMPONENT_CODESET) ? codeset : "",
				 (i & COMPONENT_MODIFIER) ? modifier : "",
				 NULL);
	retval = g_list_prepend (retval, val);
      }

  g_free (language);
  if (mask & COMPONENT_CODESET)
    g_free (codeset);
  if (mask & COMPONENT_TERRITORY)
    g_free (territory);
  if (mask & COMPONENT_MODIFIER)
    g_free (modifier);

  return retval;
}

/* The following is (partly) taken from the gettext package.
   Copyright (C) 1995, 1996, 1997, 1998 Free Software Foundation, Inc.  */

static const gchar *
guess_category_value (const gchar *categoryname)
{
  const gchar *retval;

  /* The highest priority value is the `LANGUAGE' environment
     variable.  This is a GNU extension.  */
  retval = getenv ("LANGUAGE");
  if (retval != NULL && retval[0] != '\0')
    return retval;

  /* `LANGUAGE' is not set.  So we have to proceed with the POSIX
     methods of looking to `LC_ALL', `LC_xxx', and `LANG'.  On some
     systems this can be done by the `setlocale' function itself.  */

  /* Setting of LC_ALL overwrites all other.  */
  retval = getenv ("LC_ALL");  
  if (retval != NULL && retval[0] != '\0')
    return retval;

  /* Next comes the name of the desired category.  */
  retval = getenv (categoryname);
  if (retval != NULL && retval[0] != '\0')
    return retval;

  /* Last possibility is the LANG environment variable.  */
  retval = getenv ("LANG");
  if (retval != NULL && retval[0] != '\0')
    return retval;

  return NULL;
}


static GHashTable *category_table= NULL;


/**
 * gnome_i18n_get_language_list:
 * @category_name: Name of category to look up, e.g. "LC_MESSAGES".
 * 
 * This computes a list of language strings.  It searches in the
 * standard environment variables to find the list, which is sorted
 * in order from most desirable to least desirable.  The `C' locale
 * is appended to the list if it does not already appear.
 * If @category_name is %NULL, then LC_ALL is assumed.
 * 
 * Return value: the list of languages
 **/
GList *
gnome_i18n_get_language_list (const gchar *category_name)
{
  GList *list;

  if (!category_name)
    category_name= "LC_ALL";

  if (category_table)
    {
      list= g_hash_table_lookup (category_table, (const gpointer) category_name);
    }
  else
    {
      category_table= g_hash_table_new (g_str_hash, g_str_equal);
      list= NULL;
    }

  if (!list)
    {
      gint c_locale_defined= FALSE;
  
      const gchar *category_value;
      gchar *category_memory, *orig_category_memory;

      category_value = guess_category_value (category_name);
      if (! category_value)
	category_value = "C";
      orig_category_memory = category_memory =
	g_malloc (strlen (category_value)+1);
      
      while (category_value[0] != '\0')
	{
	  while (category_value[0] != '\0' && category_value[0] == ':')
	    ++category_value;
	  
	  if (category_value[0] != '\0')
	    {
	      char *cp= category_memory;
	      
	      while (category_value[0] != '\0' && category_value[0] != ':')
		*category_memory++= *category_value++;
	      
	      category_memory[0]= '\0'; 
	      category_memory++;
	      
	      cp = unalias_lang(cp);
	      
	      if (strcmp (cp, "C") == 0)
		c_locale_defined= TRUE;
	      
	      list= g_list_concat (list, compute_locale_variants (cp));
	    }
	}

      g_free (orig_category_memory);
      
      if (!c_locale_defined)
	list= g_list_append (list, "C");

      g_hash_table_insert (category_table, (gpointer) category_name, list);
    }
  
  return list;
}

/**
 * gnome_i18n_set_preferred_language:
 * @val: Preferred language
 * 
 * This sets the user's preferred language in the Gnome config
 * database.  This value can always be overridden by the standard
 * environment variables.  It exists so that a config applet which
 * chooses the preferred language has a standard place to put the
 * resulting information.
 **/
void
gnome_i18n_set_preferred_language (const char *val)
{
  gnome_config_set_string (LANGKEY, val);
}

/**
 * gnome_i18n_init:
 * 
 * Initialize the i18n environment variables (if not already set) from
 * the Gnome config database.  Ordinarily this should not be called by
 * user code.
 **/
void
gnome_i18n_init (void)
{
  const gchar *val = guess_category_value ("LC_ALL");

  if (val == NULL)
    {
      /* No value in environment.  So we might set up environment
	 according to what is in the config database.  We do this so
	 that the user can override the config db using the
	 environment.  */
      val = gnome_config_get_string (LANGKEY);
      if (val != NULL) 
        {
#ifdef HAVE_SETENV      
	  setenv ("LC_ALL", val, 1);
#else
#ifdef HAVE_PUTENV
	  /* It is not safe to free the value passed to putenv.  */
	  putenv (g_strconcat ("LC_ALL=", val, NULL));
#endif
#endif
	}
    }
}

/**
 * gnome_i18n_get_preferred_language:
 * 
 * Return value: the preferred language as set in the Gnome config database.
 **/
const char *
gnome_i18n_get_preferred_language (void)
{
  return gnome_config_get_string (LANGKEY);
}
