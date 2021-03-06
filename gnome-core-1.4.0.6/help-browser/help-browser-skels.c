/*
 * This file was generated by orbit-idl - DO NOT EDIT!
 */

#include <string.h>
#include "help-browser.h"

void
_ORBIT_skel_help_browser_simple_browser_fetch_url
   (POA_help_browser_simple_browser * _ORBIT_servant,
    GIOPRecvBuffer * _ORBIT_recv_buffer, CORBA_Environment * ev,
    void (*_impl_fetch_url) (PortableServer_Servant _servant,
			     const CORBA_char * URL, CORBA_Environment * ev))
{
   CORBA_char *URL;

   {				/* demarshalling */
      guchar *_ORBIT_curptr;
      register CORBA_unsigned_long _ORBIT_tmpvar_2;
      CORBA_unsigned_long _ORBIT_tmpvar_3;

      _ORBIT_curptr = GIOP_RECV_BUFFER(_ORBIT_recv_buffer)->cur;
      if (giop_msg_conversion_needed(GIOP_MESSAGE_BUFFER(_ORBIT_recv_buffer))) {
	 _ORBIT_curptr = ALIGN_ADDRESS(_ORBIT_curptr, 4);
	 (*((guint32 *) & (_ORBIT_tmpvar_3))) =
	    GUINT32_SWAP_LE_BE(*((guint32 *) _ORBIT_curptr));
	 _ORBIT_curptr += 4;
	 URL = (void *) _ORBIT_curptr;
	 _ORBIT_curptr += sizeof(URL[_ORBIT_tmpvar_2]) * _ORBIT_tmpvar_3;
      } else {
	 _ORBIT_curptr = ALIGN_ADDRESS(_ORBIT_curptr, 4);
	 _ORBIT_tmpvar_3 = *((CORBA_unsigned_long *) _ORBIT_curptr);
	 _ORBIT_curptr += 4;
	 URL = (void *) _ORBIT_curptr;
	 _ORBIT_curptr += sizeof(URL[_ORBIT_tmpvar_2]) * _ORBIT_tmpvar_3;
      }
   }
   _impl_fetch_url(_ORBIT_servant, URL, ev);
   {				/* marshalling */
      register GIOPSendBuffer *_ORBIT_send_buffer;

      _ORBIT_send_buffer =
	 giop_send_reply_buffer_use(GIOP_MESSAGE_BUFFER(_ORBIT_recv_buffer)->
				    connection, NULL,
				    _ORBIT_recv_buffer->message.u.request.
				    request_id, ev->_major);
      if (_ORBIT_send_buffer) {
	 if (ev->_major == CORBA_NO_EXCEPTION) {
	 } else
	    ORBit_send_system_exception(_ORBIT_send_buffer, ev);
	 giop_send_buffer_write(_ORBIT_send_buffer);
	 giop_send_buffer_unuse(_ORBIT_send_buffer);
      }
   }
}
void
_ORBIT_skel_help_browser_simple_browser_show_url
   (POA_help_browser_simple_browser * _ORBIT_servant,
    GIOPRecvBuffer * _ORBIT_recv_buffer, CORBA_Environment * ev,
    help_browser_simple_browser(*_impl_show_url) (PortableServer_Servant
						  _servant,
						  const CORBA_char * URL,
						  CORBA_Environment * ev))
{
   help_browser_simple_browser _ORBIT_retval;
   CORBA_char *URL;

   {				/* demarshalling */
      guchar *_ORBIT_curptr;
      register CORBA_unsigned_long _ORBIT_tmpvar_2;
      CORBA_unsigned_long _ORBIT_tmpvar_3;

      _ORBIT_curptr = GIOP_RECV_BUFFER(_ORBIT_recv_buffer)->cur;
      if (giop_msg_conversion_needed(GIOP_MESSAGE_BUFFER(_ORBIT_recv_buffer))) {
	 _ORBIT_curptr = ALIGN_ADDRESS(_ORBIT_curptr, 4);
	 (*((guint32 *) & (_ORBIT_tmpvar_3))) =
	    GUINT32_SWAP_LE_BE(*((guint32 *) _ORBIT_curptr));
	 _ORBIT_curptr += 4;
	 URL = (void *) _ORBIT_curptr;
	 _ORBIT_curptr += sizeof(URL[_ORBIT_tmpvar_2]) * _ORBIT_tmpvar_3;
      } else {
	 _ORBIT_curptr = ALIGN_ADDRESS(_ORBIT_curptr, 4);
	 _ORBIT_tmpvar_3 = *((CORBA_unsigned_long *) _ORBIT_curptr);
	 _ORBIT_curptr += 4;
	 URL = (void *) _ORBIT_curptr;
	 _ORBIT_curptr += sizeof(URL[_ORBIT_tmpvar_2]) * _ORBIT_tmpvar_3;
      }
   }
   _ORBIT_retval = _impl_show_url(_ORBIT_servant, URL, ev);
   {				/* marshalling */
      register GIOPSendBuffer *_ORBIT_send_buffer;

      _ORBIT_send_buffer =
	 giop_send_reply_buffer_use(GIOP_MESSAGE_BUFFER(_ORBIT_recv_buffer)->
				    connection, NULL,
				    _ORBIT_recv_buffer->message.u.request.
				    request_id, ev->_major);
      if (_ORBIT_send_buffer) {
	 if (ev->_major == CORBA_NO_EXCEPTION) {
	    ORBit_marshal_object(_ORBIT_send_buffer, _ORBIT_retval);
	 } else
	    ORBit_send_system_exception(_ORBIT_send_buffer, ev);
	 giop_send_buffer_write(_ORBIT_send_buffer);
	 giop_send_buffer_unuse(_ORBIT_send_buffer);
      }
      if (ev->_major == CORBA_NO_EXCEPTION)
	 CORBA_Object_release(_ORBIT_retval, ev);
   }
}
static ORBitSkeleton
get_skel_help_browser_simple_browser(POA_help_browser_simple_browser *
				     servant,
				     GIOPRecvBuffer * _ORBIT_recv_buffer,
				     gpointer * impl)
{
   gchar *opname = _ORBIT_recv_buffer->message.u.request.operation;

   switch (opname[0]) {
     case 'f':
	if (strcmp((opname + 1), "etch_url"))
	   break;
	*impl =
	   (gpointer) servant->vepv->help_browser_simple_browser_epv->
	   fetch_url;
	return (ORBitSkeleton)
	   _ORBIT_skel_help_browser_simple_browser_fetch_url;
	break;
     case 's':
	if (strcmp((opname + 1), "how_url"))
	   break;
	*impl =
	   (gpointer) servant->vepv->help_browser_simple_browser_epv->
	   show_url;
	return (ORBitSkeleton)
	   _ORBIT_skel_help_browser_simple_browser_show_url;
	break;
     default:
	break;
   }
   return NULL;
}

static void
init_local_objref_help_browser_simple_browser(CORBA_Object obj,
					      POA_help_browser_simple_browser
					      * servant)
{
   obj->vepv[help_browser_simple_browser__classid] =
      servant->vepv->help_browser_simple_browser_epv;
}

void
POA_help_browser_simple_browser__init(PortableServer_Servant servant,
				      CORBA_Environment * env)
{
   static const PortableServer_ClassInfo class_info =
      { (ORBit_impl_finder) & get_skel_help_browser_simple_browser,
"IDL:help_browser/simple_browser:1.0", (ORBit_local_objref_init) & init_local_objref_help_browser_simple_browser };
   PortableServer_ServantBase__init(((PortableServer_ServantBase *) servant),
				    env);
   ORBIT_OBJECT_KEY(((PortableServer_ServantBase *) servant)->_private)->
      class_info = (PortableServer_ClassInfo *) & class_info;
   if (!help_browser_simple_browser__classid)
      help_browser_simple_browser__classid =
	 ORBit_register_class(&class_info);
}

void
POA_help_browser_simple_browser__fini(PortableServer_Servant servant,
				      CORBA_Environment * env)
{
   PortableServer_ServantBase__fini(servant, env);
}
