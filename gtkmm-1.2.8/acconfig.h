#ifndef _ACCONFIG_H
#define _ACCONFIG_H 1

/* package & version name */
#undef VERSION
#undef PACKAGE

/* feature tests that are used during library creation only */
#undef HAVE_WORKING_BASENAME

/* autoheader reqiures all defines inside acconfig.h, so all
   defines from g[dt]k--config.h are added here inside of #if 0 */
#if 0
#undef GTKMM_CXX_STD_NAMESPACE
#undef GTKMM_CXX_GAUB
#undef GTKMM_CXX_AMBIGUOUS_TEMPLATES
#undef GTKMM_MAJOR_VERSION
#undef GTKMM_MICRO_VERSION
#undef GTKMM_MINOR_VERSION

/* Using decls for names in std.*/
/*
  Make sure there is at least one space at the end of each of these #defines 
    If there is not, the autoheader sed script will not insert the
    values these defines are suppoed to have.
 */
#define GTKMM_USING_STD_STRING 
#define GTKMM_USING_STD_MAP 
#define GTKMM_USING_STD_BIDIRECTIONAL_ITERATOR_TAG  
#define GTKMM_USING_STD_FORWARD_ITERATOR_TAG  
#define GTKMM_USING_STD_INPUT_ITERATOR_TAG  

#endif

/* autoheader adds more tests below this */
@TOP@
@BOTTOM@

#endif /* _ACCONFIG_H */



