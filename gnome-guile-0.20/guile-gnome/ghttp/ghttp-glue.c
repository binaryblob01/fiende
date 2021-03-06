/* Generated by build-guile-gtk from "./ghttp.defs".  Do not edit. */

#include <libguile.h>
#include <guile-gtk.h>
#include <ghttp.h>

static sgtk_enum_literal _ghttp_type_literals[15] = {
  { SCM_UNDEFINED, "get", ghttp_type_get },
  { SCM_UNDEFINED, "opitons", ghttp_type_options },
  { SCM_UNDEFINED, "head", ghttp_type_head },
  { SCM_UNDEFINED, "post", ghttp_type_post },
  { SCM_UNDEFINED, "put", ghttp_type_put },
  { SCM_UNDEFINED, "delete", ghttp_type_delete },
  { SCM_UNDEFINED, "trace", ghttp_type_trace },
  { SCM_UNDEFINED, "connect", ghttp_type_connect },
  { SCM_UNDEFINED, "propfind", ghttp_type_propfind },
  { SCM_UNDEFINED, "propatch", ghttp_type_proppatch },
  { SCM_UNDEFINED, "mkcol", ghttp_type_mkcol },
  { SCM_UNDEFINED, "copy", ghttp_type_copy },
  { SCM_UNDEFINED, "move", ghttp_type_move },
  { SCM_UNDEFINED, "lock", ghttp_type_lock },
  { SCM_UNDEFINED, "unlock", ghttp_type_unlock },
};
sgtk_enum_info sgtk_ghttp_type_info = {
  { "ghttptype", GTK_TYPE_ENUM }, 15, _ghttp_type_literals,
};

static sgtk_enum_literal _ghttp_sync_mode_literals[2] = {
  { SCM_UNDEFINED, "sync", ghttp_sync },
  { SCM_UNDEFINED, "async", ghttp_async },
};
sgtk_enum_info sgtk_ghttp_sync_mode_info = {
  { "ghttpsyncmode", GTK_TYPE_ENUM }, 2, _ghttp_sync_mode_literals,
};

static sgtk_enum_literal _ghttp_status_literals[3] = {
  { SCM_UNDEFINED, "error", ghttp_error },
  { SCM_UNDEFINED, "not-done", ghttp_not_done },
  { SCM_UNDEFINED, "donde", ghttp_done },
};
sgtk_enum_info sgtk_ghttp_status_info = {
  { "ghttpstatus", GTK_TYPE_ENUM }, 3, _ghttp_status_literals,
};

static sgtk_enum_literal _ghttp_proc_literals[4] = {
  { SCM_UNDEFINED, "none", ghttp_proc_none },
  { SCM_UNDEFINED, "request", ghttp_proc_request },
  { SCM_UNDEFINED, "hdrs", ghttp_proc_response_hdrs },
  { SCM_UNDEFINED, "response", ghttp_proc_response },
};
sgtk_enum_info sgtk_ghttp_proc_info = {
  { "ghttpproc", GTK_TYPE_ENUM }, 4, _ghttp_proc_literals,
};

GtkTypeInfo sgtk_ghttp_request_info_gtk = {
  "ghttprequest", "sizeof (ghttp_request)", 0,
  (GtkClassInitFunc) NULL,
  (GtkObjectInitFunc) NULL,
  (GtkArgSetFunc) NULL,
  (GtkArgGetFunc) NULL,
  (GtkClassInitFunc) NULL,
};

sgtk_boxed_info sgtk_ghttp_request_info = {
  { "ghttprequest", GTK_TYPE_BOXED, NULL },
  (void *(*)(void*))ghttp_request_new,
  (void (*)(void*))ghttp_request_destroy,
  0
};

static sgtk_type_info *type_infos[] = {
  (sgtk_type_info*)&sgtk_ghttp_request_info,
  (sgtk_type_info*)&sgtk_ghttp_type_info,
  (sgtk_type_info*)&sgtk_ghttp_sync_mode_info,
  (sgtk_type_info*)&sgtk_ghttp_status_info,
  (sgtk_type_info*)&sgtk_ghttp_proc_info,
  NULL
};

static GtkTypeInfo *type_infos_gtk[] = {
  (GtkTypeInfo*)&sgtk_ghttp_request_info_gtk,
  NULL
};

static char s_ghttp_request_new[] = "ghttp-request-new";

SCM
sgtk_ghttp_request_new ()
{
  ghttp_request* cr_ret;

  SCM_DEFER_INTS;
  cr_ret = ghttp_request_new ();
  SCM_ALLOW_INTS;

  return sgtk_boxed2scm (cr_ret, &sgtk_ghttp_request_info, 1);
}

static char s_ghttp_request_destroy[] = "ghttp-request-destroy";

SCM
sgtk_ghttp_request_destroy (SCM p_a_request)
{
  ghttp_request* c_a_request;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_request_destroy);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  ghttp_request_destroy (c_a_request);
  SCM_ALLOW_INTS;

  return SCM_UNSPECIFIED;
}

static char s_ghttp_uri_validate[] = "ghttp-uri-validate";

SCM
sgtk_ghttp_uri_validate (SCM p_a_uri)
{
  gint cr_ret;
  char* c_a_uri;
  p_a_uri = sgtk_string_conversion (p_a_uri);
  SCM_ASSERT ((SCM_NIMP(p_a_uri) && SCM_RWSTRINGP(p_a_uri)), p_a_uri, SCM_ARG1, s_ghttp_uri_validate);

  SCM_DEFER_INTS;
  c_a_uri = ((p_a_uri) == SCM_BOOL_F? NULL : SCM_CHARS(p_a_uri));
  cr_ret = ghttp_uri_validate (c_a_uri);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

static char s_ghttp_set_uri[] = "ghttp-set-uri";

SCM
sgtk_ghttp_set_uri (SCM p_a_request, SCM p_a_uri)
{
  gint cr_ret;
  ghttp_request* c_a_request;
  char* c_a_uri;
  p_a_uri = sgtk_string_conversion (p_a_uri);
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_set_uri);
  SCM_ASSERT ((SCM_NIMP(p_a_uri) && SCM_RWSTRINGP(p_a_uri)), p_a_uri, SCM_ARG2, s_ghttp_set_uri);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  c_a_uri = ((p_a_uri) == SCM_BOOL_F? NULL : SCM_CHARS(p_a_uri));
  cr_ret = ghttp_set_uri (c_a_request, c_a_uri);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

static char s_ghttp_set_proxy[] = "ghttp-set-proxy";

SCM
sgtk_ghttp_set_proxy (SCM p_a_request, SCM p_a_uri)
{
  gint cr_ret;
  ghttp_request* c_a_request;
  char* c_a_uri;
  p_a_uri = sgtk_string_conversion (p_a_uri);
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_set_proxy);
  SCM_ASSERT ((SCM_NIMP(p_a_uri) && SCM_RWSTRINGP(p_a_uri)), p_a_uri, SCM_ARG2, s_ghttp_set_proxy);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  c_a_uri = ((p_a_uri) == SCM_BOOL_F? NULL : SCM_CHARS(p_a_uri));
  cr_ret = ghttp_set_proxy (c_a_request, c_a_uri);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

static char s_ghttp_set_type[] = "ghttp-set-type";

SCM
sgtk_ghttp_set_type (SCM p_a_request, SCM p_a_type)
{
  gint cr_ret;
  ghttp_request* c_a_request;
  ghttp_type c_a_type;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_set_type);
  c_a_type = sgtk_scm2enum (p_a_type, &sgtk_ghttp_type_info, SCM_ARG2, s_ghttp_set_type);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  ;
  cr_ret = ghttp_set_type (c_a_request, c_a_type);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

static char s_ghttp_set_body[] = "ghttp-set-body";

SCM
sgtk_ghttp_set_body (SCM p_a_request, SCM p_a_body, SCM p_a_len)
{
  gint cr_ret;
  ghttp_request* c_a_request;
  char* c_a_body;
  gint c_a_len;
  p_a_body = sgtk_string_conversion (p_a_body);
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_set_body);
  SCM_ASSERT ((SCM_NIMP(p_a_body) && SCM_RWSTRINGP(p_a_body)), p_a_body, SCM_ARG2, s_ghttp_set_body);
  c_a_len = scm_num2long (p_a_len, (char *)SCM_ARG3, s_ghttp_set_body);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  c_a_body = ((p_a_body) == SCM_BOOL_F? NULL : SCM_CHARS(p_a_body));
  ;
  cr_ret = ghttp_set_body (c_a_request, c_a_body, c_a_len);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

static char s_ghttp_set_sync[] = "ghttp-set-sync";

SCM
sgtk_ghttp_set_sync (SCM p_a_request, SCM p_a_mode)
{
  gint cr_ret;
  ghttp_request* c_a_request;
  ghttp_sync_mode c_a_mode;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_set_sync);
  c_a_mode = sgtk_scm2enum (p_a_mode, &sgtk_ghttp_sync_mode_info, SCM_ARG2, s_ghttp_set_sync);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  ;
  cr_ret = ghttp_set_sync (c_a_request, c_a_mode);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

static char s_ghttp_prepare[] = "ghttp-prepare";

SCM
sgtk_ghttp_prepare (SCM p_a_request)
{
  gint cr_ret;
  ghttp_request* c_a_request;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_prepare);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  cr_ret = ghttp_prepare (c_a_request);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

static char s_ghttp_set_chunksize[] = "ghttp-set-chunksize";

SCM
sgtk_ghttp_set_chunksize (SCM p_a_request, SCM p_a_size)
{
  ghttp_request* c_a_request;
  gint c_a_size;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_set_chunksize);
  c_a_size = scm_num2long (p_a_size, (char *)SCM_ARG2, s_ghttp_set_chunksize);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  ;
  ghttp_set_chunksize (c_a_request, c_a_size);
  SCM_ALLOW_INTS;

  return SCM_UNSPECIFIED;
}

static char s_ghttp_set_header[] = "ghttp-set-header";

SCM
sgtk_ghttp_set_header (SCM p_a_request, SCM p_a_hdr, SCM p_a_val)
{
  ghttp_request* c_a_request;
  char* c_a_hdr;
  char* c_a_val;
  p_a_hdr = sgtk_string_conversion (p_a_hdr);
  p_a_val = sgtk_string_conversion (p_a_val);
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_set_header);
  SCM_ASSERT ((SCM_NIMP(p_a_hdr) && SCM_RWSTRINGP(p_a_hdr)), p_a_hdr, SCM_ARG2, s_ghttp_set_header);
  SCM_ASSERT ((SCM_NIMP(p_a_val) && SCM_RWSTRINGP(p_a_val)), p_a_val, SCM_ARG3, s_ghttp_set_header);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  c_a_hdr = ((p_a_hdr) == SCM_BOOL_F? NULL : SCM_CHARS(p_a_hdr));
  c_a_val = ((p_a_val) == SCM_BOOL_F? NULL : SCM_CHARS(p_a_val));
  ghttp_set_header (c_a_request, c_a_hdr, c_a_val);
  SCM_ALLOW_INTS;

  return SCM_UNSPECIFIED;
}

static char s_ghttp_process[] = "ghttp-process";

SCM
sgtk_ghttp_process (SCM p_a_request)
{
  ghttp_status cr_ret;
  ghttp_request* c_a_request;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_process);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  cr_ret = ghttp_process (c_a_request);
  SCM_ALLOW_INTS;

  return sgtk_enum2scm (cr_ret, &sgtk_ghttp_status_info);
}

static char s_ghttp_get_header[] = "ghttp-get-header";

SCM
sgtk_ghttp_get_header (SCM p_a_request, SCM p_a_hdr)
{
  char* cr_ret;
  ghttp_request* c_a_request;
  char* c_a_hdr;
  p_a_hdr = sgtk_string_conversion (p_a_hdr);
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_get_header);
  SCM_ASSERT ((SCM_NIMP(p_a_hdr) && SCM_RWSTRINGP(p_a_hdr)), p_a_hdr, SCM_ARG2, s_ghttp_get_header);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  c_a_hdr = ((p_a_hdr) == SCM_BOOL_F? NULL : SCM_CHARS(p_a_hdr));
  cr_ret = ghttp_get_header (c_a_request, c_a_hdr);
  SCM_ALLOW_INTS;

  return (cr_ret == NULL? SCM_BOOL_F : scm_take0str (cr_ret));
}

static char s_ghttp_close[] = "ghttp-close";

SCM
sgtk_ghttp_close (SCM p_a_request)
{
  gint cr_ret;
  ghttp_request* c_a_request;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_close);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  cr_ret = ghttp_close (c_a_request);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

static char s_ghttp_clean[] = "ghttp-clean";

SCM
sgtk_ghttp_clean (SCM p_a_request)
{
  ghttp_request* c_a_request;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_clean);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  ghttp_clean (c_a_request);
  SCM_ALLOW_INTS;

  return SCM_UNSPECIFIED;
}

static char s_ghttp_get_socket[] = "ghttp-get-socket";

SCM
sgtk_ghttp_get_socket (SCM p_a_request)
{
  gint cr_ret;
  ghttp_request* c_a_request;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_get_socket);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  cr_ret = ghttp_get_socket (c_a_request);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

static char s_ghttp_get_body[] = "ghttp-get-body";

SCM
sgtk_ghttp_get_body (SCM p_a_request)
{
  char* cr_ret;
  ghttp_request* c_a_request;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_get_body);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  cr_ret = ghttp_get_body (c_a_request);
  SCM_ALLOW_INTS;

  return (cr_ret == NULL? SCM_BOOL_F : scm_take0str (cr_ret));
}

static char s_ghttp_get_body_len[] = "ghttp-get-body-len";

SCM
sgtk_ghttp_get_body_len (SCM p_a_request)
{
  gint cr_ret;
  ghttp_request* c_a_request;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_get_body_len);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  cr_ret = ghttp_get_body_len (c_a_request);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

static char s_ghttp_get_error[] = "ghttp-get-error";

SCM
sgtk_ghttp_get_error (SCM p_a_request)
{
  char* cr_ret;
  ghttp_request* c_a_request;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_get_error);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  cr_ret = ghttp_get_error (c_a_request);
  SCM_ALLOW_INTS;

  return (cr_ret == NULL? SCM_BOOL_F : scm_take0str (cr_ret));
}

static char s_ghttp_status_code[] = "ghttp-status-code";

SCM
sgtk_ghttp_status_code (SCM p_a_request)
{
  gint cr_ret;
  ghttp_request* c_a_request;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_status_code);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  cr_ret = ghttp_status_code (c_a_request);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

static char s_ghttp_reason_phrase[] = "ghttp-reason-phrase";

SCM
sgtk_ghttp_reason_phrase (SCM p_a_request)
{
  char* cr_ret;
  ghttp_request* c_a_request;
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_reason_phrase);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  cr_ret = ghttp_reason_phrase (c_a_request);
  SCM_ALLOW_INTS;

  return (cr_ret == NULL? SCM_BOOL_F : scm_take0str (cr_ret));
}

static char s_ghttp_set_authinfo[] = "ghttp-set-authinfo";

SCM
sgtk_ghttp_set_authinfo (SCM p_a_request, SCM p_a_user, SCM p_a_pass)
{
  gint cr_ret;
  ghttp_request* c_a_request;
  char* c_a_user;
  char* c_a_pass;
  p_a_user = sgtk_string_conversion (p_a_user);
  p_a_pass = sgtk_string_conversion (p_a_pass);
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_set_authinfo);
  SCM_ASSERT ((SCM_NIMP(p_a_user) && SCM_RWSTRINGP(p_a_user)), p_a_user, SCM_ARG2, s_ghttp_set_authinfo);
  SCM_ASSERT ((SCM_NIMP(p_a_pass) && SCM_RWSTRINGP(p_a_pass)), p_a_pass, SCM_ARG3, s_ghttp_set_authinfo);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  c_a_user = ((p_a_user) == SCM_BOOL_F? NULL : SCM_CHARS(p_a_user));
  c_a_pass = ((p_a_pass) == SCM_BOOL_F? NULL : SCM_CHARS(p_a_pass));
  cr_ret = ghttp_set_authinfo (c_a_request, c_a_user, c_a_pass);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

static char s_ghttp_set_proxy_authinfo[] = "ghttp-set-proxy-authinfo";

SCM
sgtk_ghttp_set_proxy_authinfo (SCM p_a_request, SCM p_a_user, SCM p_a_pass)
{
  gint cr_ret;
  ghttp_request* c_a_request;
  char* c_a_user;
  char* c_a_pass;
  p_a_user = sgtk_string_conversion (p_a_user);
  p_a_pass = sgtk_string_conversion (p_a_pass);
  SCM_ASSERT (sgtk_valid_boxed (p_a_request, &sgtk_ghttp_request_info), p_a_request, SCM_ARG1, s_ghttp_set_proxy_authinfo);
  SCM_ASSERT ((SCM_NIMP(p_a_user) && SCM_RWSTRINGP(p_a_user)), p_a_user, SCM_ARG2, s_ghttp_set_proxy_authinfo);
  SCM_ASSERT ((SCM_NIMP(p_a_pass) && SCM_RWSTRINGP(p_a_pass)), p_a_pass, SCM_ARG3, s_ghttp_set_proxy_authinfo);

  SCM_DEFER_INTS;
  c_a_request = (ghttp_request*)sgtk_scm2boxed (p_a_request);
  c_a_user = ((p_a_user) == SCM_BOOL_F? NULL : SCM_CHARS(p_a_user));
  c_a_pass = ((p_a_pass) == SCM_BOOL_F? NULL : SCM_CHARS(p_a_pass));
  cr_ret = ghttp_set_proxy_authinfo (c_a_request, c_a_user, c_a_pass);
  SCM_ALLOW_INTS;

  return scm_long2num (cr_ret);
}

void
ghttp_init_glue_types ()
{
  static int done = 0;
  if (!done)
    {
      done = 1;
      sgtk_register_type_infos (type_infos);
      sgtk_register_type_infos_gtk (type_infos_gtk);

      sgtk_enum_flags_init (&sgtk_ghttp_proc_info);
      sgtk_enum_flags_init (&sgtk_ghttp_status_info);
      sgtk_enum_flags_init (&sgtk_ghttp_sync_mode_info);
      sgtk_enum_flags_init (&sgtk_ghttp_type_info);
    }
}


void
ghttp_init_glue ()
{
  ghttp_init_glue_types ();
  scm_make_gsubr (s_ghttp_set_proxy_authinfo, 3, 0, 0, sgtk_ghttp_set_proxy_authinfo);
  scm_make_gsubr (s_ghttp_set_authinfo, 3, 0, 0, sgtk_ghttp_set_authinfo);
  scm_make_gsubr (s_ghttp_reason_phrase, 1, 0, 0, sgtk_ghttp_reason_phrase);
  scm_make_gsubr (s_ghttp_status_code, 1, 0, 0, sgtk_ghttp_status_code);
  scm_make_gsubr (s_ghttp_get_error, 1, 0, 0, sgtk_ghttp_get_error);
  scm_make_gsubr (s_ghttp_get_body_len, 1, 0, 0, sgtk_ghttp_get_body_len);
  scm_make_gsubr (s_ghttp_get_body, 1, 0, 0, sgtk_ghttp_get_body);
  scm_make_gsubr (s_ghttp_get_socket, 1, 0, 0, sgtk_ghttp_get_socket);
  scm_make_gsubr (s_ghttp_clean, 1, 0, 0, sgtk_ghttp_clean);
  scm_make_gsubr (s_ghttp_close, 1, 0, 0, sgtk_ghttp_close);
  scm_make_gsubr (s_ghttp_get_header, 2, 0, 0, sgtk_ghttp_get_header);
  scm_make_gsubr (s_ghttp_process, 1, 0, 0, sgtk_ghttp_process);
  scm_make_gsubr (s_ghttp_set_header, 3, 0, 0, sgtk_ghttp_set_header);
  scm_make_gsubr (s_ghttp_set_chunksize, 2, 0, 0, sgtk_ghttp_set_chunksize);
  scm_make_gsubr (s_ghttp_prepare, 1, 0, 0, sgtk_ghttp_prepare);
  scm_make_gsubr (s_ghttp_set_sync, 2, 0, 0, sgtk_ghttp_set_sync);
  scm_make_gsubr (s_ghttp_set_body, 3, 0, 0, sgtk_ghttp_set_body);
  scm_make_gsubr (s_ghttp_set_type, 2, 0, 0, sgtk_ghttp_set_type);
  scm_make_gsubr (s_ghttp_set_proxy, 2, 0, 0, sgtk_ghttp_set_proxy);
  scm_make_gsubr (s_ghttp_set_uri, 2, 0, 0, sgtk_ghttp_set_uri);
  scm_make_gsubr (s_ghttp_uri_validate, 1, 0, 0, sgtk_ghttp_uri_validate);
  scm_make_gsubr (s_ghttp_request_destroy, 1, 0, 0, sgtk_ghttp_request_destroy);
  scm_make_gsubr (s_ghttp_request_new, 0, 0, 0, sgtk_ghttp_request_new);
}
