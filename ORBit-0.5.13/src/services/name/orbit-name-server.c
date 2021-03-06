/* 
 *  orbit-name-server : a CORBA CosNaming server
 *
 *  Copyright (C) 1998 Sebastian Wilhelmi; University of Karlsruhe
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "CosNaming.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>

#define THROW(name,arg,ev)						\
  CORBA_exception_set( ev, CORBA_USER_EXCEPTION, ex_##name, arg ),	\
  syslog(LOG_ERR,"ERROR: %s thrown.\n", ex_##name );

#define RETHROW(ev, val ) if ((ev)->_major != CORBA_NO_EXCEPTION ) return val;
#define RETHROW_VOID(ev) if ((ev)->_major != CORBA_NO_EXCEPTION ) return;

#define THROW_NotFound(reason, name, ev)				\
  G_STMT_START {							\
    CosNaming_NamingContext_NotFound* exception =			\
      CosNaming_NamingContext_NotFound__alloc ();			\
    exception->why = CosNaming_NamingContext_##reason;			\
    Name__copy ( &exception->rest_of_name, name, 0 );			\
    THROW (CosNaming_NamingContext_NotFound, exception, ev);		\
  } G_STMT_END

#define THROW_CannotProceed(context, name, ev )				\
  G_STMT_START {							\
    CosNaming_NamingContext_CannotProceed* exception =			\
      CosNaming_NamingContext_CannotProceed__alloc ();			\
    exception->ctx = CORBA_Object_duplicate( context, ev );		\
    Name__copy ( &exception->rest_of_name, name, 0 );			\
    THROW (CosNaming_NamingContext_NotFound, exception, ev);		\
  } G_STMT_END

#define THROW_InvalidName(ev)						\
   THROW(CosNaming_NamingContext_InvalidName, NULL, ev) 

#define THROW_AlreadyBound(ev)						\
   THROW(CosNaming_NamingContext_AlreadyBound, NULL, ev) 

#define THROW_NotEmpty(ev)						\
   THROW(CosNaming_NamingContext_NotEmpty, NULL, ev) 

typedef enum
  {
    result_ok, result_lower_level, result_exception,
    result_not_found
  }
ActionReturnType;

/* hash and comparision function for a NameComponent */

static guint
CosNaming_NameComponent__hash(gconstpointer v)
{
  CosNaming_NameComponent const* c = v;
  return g_str_hash( c->id ) ^ g_str_hash( c->kind );
}

static gint
CosNaming_NameComponent__equal(gconstpointer v1, gconstpointer v2)
{
  CosNaming_NameComponent const* c1 = v1;
  CosNaming_NameComponent const* c2 = v2;
  return strcmp( c1->id , c2->id ) == 0 && strcmp( c1->kind , c2->kind ) == 0;
}

/* The following is mostly taken from CosNaming-impl.c as generated by
   "orbit-idl -Eskeleton_impl CosNaming.idl", only the stub
   implemementations have been removed and some lines have been added,
   those are marked with "added" */

/***************************************************************************/
/*                         begin of generated part                         */
/***************************************************************************/

/*** App-specific servant structures ***/

typedef struct
  {
    POA_CosNaming_NamingContext servant;
    PortableServer_POA poa;
    GHashTable *table;               /* added */
  }
impl_POA_CosNaming_NamingContext;

typedef struct
  {
    POA_CosNaming_BindingIterator servant;
    PortableServer_POA poa;
    GArray *array;                    /* added */
    guint transmitted;                /* added */
  }
impl_POA_CosNaming_BindingIterator;

/*** Implementation stub prototypes ***/

static void impl_CosNaming_NamingContext__destroy (impl_POA_CosNaming_NamingContext * servant,
				       CORBA_Environment * ev);

void
impl_CosNaming_NamingContext_bind (impl_POA_CosNaming_NamingContext * servant,
				     CosNaming_Name * n,
				     CORBA_Object obj,
				     CORBA_Environment * ev);

void
impl_CosNaming_NamingContext_rebind (impl_POA_CosNaming_NamingContext * servant,
				       CosNaming_Name * n,
				       CORBA_Object obj,
				       CORBA_Environment * ev);

void
  impl_CosNaming_NamingContext_bind_context (impl_POA_CosNaming_NamingContext * servant,
					     CosNaming_Name * n,
					     CosNaming_NamingContext nc,
					     CORBA_Environment * ev);

void
  impl_CosNaming_NamingContext_rebind_context (impl_POA_CosNaming_NamingContext * servant,
					       CosNaming_Name * n,
					       CosNaming_NamingContext nc,
					       CORBA_Environment * ev);

CORBA_Object
impl_CosNaming_NamingContext_resolve (impl_POA_CosNaming_NamingContext * servant,
				      CosNaming_Name * n,
				      CORBA_Environment * ev);

void
  impl_CosNaming_NamingContext_unbind (impl_POA_CosNaming_NamingContext * servant,
				       CosNaming_Name * n,
				       CORBA_Environment * ev);

CosNaming_NamingContext
impl_CosNaming_NamingContext_new_context (impl_POA_CosNaming_NamingContext * servant,
					  CORBA_Environment * ev);

CosNaming_NamingContext
impl_CosNaming_NamingContext_bind_new_context (impl_POA_CosNaming_NamingContext * servant,
					       CosNaming_Name * n,
					       CORBA_Environment * ev);

void
  impl_CosNaming_NamingContext_destroy (impl_POA_CosNaming_NamingContext * servant,
					CORBA_Environment * ev);

void
  impl_CosNaming_NamingContext_list (impl_POA_CosNaming_NamingContext * servant,
				     CORBA_unsigned_long how_many,
				     CosNaming_BindingList ** bl,
				     CosNaming_BindingIterator * bi,
				     CORBA_Environment * ev);

static void impl_CosNaming_BindingIterator__destroy (impl_POA_CosNaming_BindingIterator * servant,
						     CORBA_Environment * ev);

CORBA_boolean
impl_CosNaming_BindingIterator_next_one (impl_POA_CosNaming_BindingIterator * servant,
					 CosNaming_Binding ** b,
					 CORBA_Environment * ev);

CORBA_boolean
impl_CosNaming_BindingIterator_next_n (impl_POA_CosNaming_BindingIterator * servant,
				       CORBA_unsigned_long how_many,
				       CosNaming_BindingList ** bl,
				       CORBA_Environment * ev);

void
  impl_CosNaming_BindingIterator_destroy (impl_POA_CosNaming_BindingIterator * servant,
					  CORBA_Environment * ev);

/*** epv structures ***/

static PortableServer_ServantBase__epv impl_CosNaming_NamingContext_base_epv =
{
  NULL,				/* _private data */
  (gpointer) & impl_CosNaming_NamingContext__destroy,	/* finalize routine */
  NULL,				/* default_POA routine */
};
static POA_CosNaming_NamingContext__epv impl_CosNaming_NamingContext_epv =
{
  NULL,				/* _private */

  (gpointer) & impl_CosNaming_NamingContext_bind,

  (gpointer) & impl_CosNaming_NamingContext_rebind,

  (gpointer) & impl_CosNaming_NamingContext_bind_context,

  (gpointer) & impl_CosNaming_NamingContext_rebind_context,

  (gpointer) & impl_CosNaming_NamingContext_resolve,

  (gpointer) & impl_CosNaming_NamingContext_unbind,

  (gpointer) & impl_CosNaming_NamingContext_new_context,

  (gpointer) & impl_CosNaming_NamingContext_bind_new_context,

  (gpointer) & impl_CosNaming_NamingContext_destroy,

  (gpointer) & impl_CosNaming_NamingContext_list,

};
static PortableServer_ServantBase__epv impl_CosNaming_BindingIterator_base_epv =
{
  NULL,				/* _private data */
  (gpointer) & impl_CosNaming_BindingIterator__destroy,		/* finalize routine */
  NULL,				/* default_POA routine */
};
static POA_CosNaming_BindingIterator__epv impl_CosNaming_BindingIterator_epv =
{
  NULL,				/* _private */
  (gpointer) & impl_CosNaming_BindingIterator_next_one,

  (gpointer) & impl_CosNaming_BindingIterator_next_n,

  (gpointer) & impl_CosNaming_BindingIterator_destroy,

};

/*** vepv structures ***/

static POA_CosNaming_NamingContext__vepv impl_CosNaming_NamingContext_vepv =
{
  &impl_CosNaming_NamingContext_base_epv,
  &impl_CosNaming_NamingContext_epv,
};
static POA_CosNaming_BindingIterator__vepv impl_CosNaming_BindingIterator_vepv =
{
  &impl_CosNaming_BindingIterator_base_epv,
  &impl_CosNaming_BindingIterator_epv,
};


/*** Stub implementations ***/

static CosNaming_NamingContext
impl_CosNaming_NamingContext__create (PortableServer_POA poa, CORBA_Environment * ev)
{
  CosNaming_NamingContext retval;
  impl_POA_CosNaming_NamingContext *newservant;
  PortableServer_ObjectId *objid;

  newservant = g_new0 (impl_POA_CosNaming_NamingContext, 1);
  newservant->servant.vepv = &impl_CosNaming_NamingContext_vepv;
  newservant->poa = poa;
 
  newservant->table = g_hash_table_new( CosNaming_NameComponent__hash, 
					CosNaming_NameComponent__equal); 
  /* added  */

  POA_CosNaming_NamingContext__init ((PortableServer_Servant) newservant, ev);
  objid = PortableServer_POA_activate_object (poa, newservant, ev);
  CORBA_free (objid);
  retval = PortableServer_POA_servant_to_reference (poa, newservant, ev);
  
  return retval;
}

/* You shouldn't call this routine directly without first deactivating the servant... */
static void
impl_CosNaming_NamingContext__destroy (impl_POA_CosNaming_NamingContext * servant, CORBA_Environment * ev)
{

  POA_CosNaming_NamingContext__fini ((PortableServer_Servant) servant, ev);
  g_free (servant);
}

static CosNaming_BindingIterator
impl_CosNaming_BindingIterator__create (PortableServer_POA poa, 
					impl_POA_CosNaming_BindingIterator** servant, /* added */
					CORBA_Environment * ev) 
{
  CosNaming_BindingIterator retval;
  impl_POA_CosNaming_BindingIterator *newservant;
  PortableServer_ObjectId *objid;

  newservant = g_new0 (impl_POA_CosNaming_BindingIterator, 1);
  newservant->servant.vepv = &impl_CosNaming_BindingIterator_vepv;
  newservant->poa = poa;

  *servant = newservant; /* added  */

  POA_CosNaming_BindingIterator__init ((PortableServer_Servant) newservant, ev);
  objid = PortableServer_POA_activate_object (poa, newservant, ev);
  CORBA_free (objid);
  retval = PortableServer_POA_servant_to_reference (poa, newservant, ev);

  return retval;
}

/* You shouldn't call this routine directly without first deactivating the servant... */
static void
impl_CosNaming_BindingIterator__destroy (impl_POA_CosNaming_BindingIterator * servant, CORBA_Environment * ev)
{

  POA_CosNaming_BindingIterator__fini ((PortableServer_Servant) servant, ev);
  g_free (servant);
}

/***************************************************************************/
/*                           end of generated part                         */
/***************************************************************************/

static void
NameComponent__copy (CosNaming_NameComponent * dest,
		     CosNaming_NameComponent * src)
{
  dest->id = CORBA_string_alloc (strlen (src->id));
  dest->kind = CORBA_string_alloc (strlen (src->kind));
  strcpy (dest->id, src->id);
  strcpy (dest->kind, src->kind);
}

static void
Name__copy ( CosNaming_Name* dest, CosNaming_Name* src, int from_pos )
{
  int i;
  g_assert( src->_length > from_pos );
  dest->_length = src->_length - from_pos;
  dest->_buffer =
    CORBA_sequence_CosNaming_NameComponent_allocbuf (dest->_length);
  for (i = 0; i < dest->_length; i++)
    {
      NameComponent__copy (dest->_buffer + i, src->_buffer + i + from_pos);
    }
}

static gchar*
NamingName2String(CosNaming_Name* name)
{
  static GString *bfr = NULL;
  gint idx;

  if(bfr)
    g_string_assign(bfr, "");
  else
    bfr = g_string_new("");

  for (idx = 0; idx < name->_length; idx++)
    {
      g_string_sprintfa(bfr, "/%s.%s",
			   name->_buffer[idx].id, name->_buffer[idx].kind);
    }

  return bfr->str;
}

typedef struct _TableEntry TableEntry;
struct _TableEntry
  {
    CosNaming_NameComponent name;
    CosNaming_BindingType type;
    CORBA_Object object;
  };

static gint
server_is_alive(CORBA_Object server)
{
  GIOPConnection* cnx = _ORBit_object_get_connection(server);
  syslog(LOG_DEBUG, "server_is_alive: cnx = %p\n", cnx);
  if (server->connection)
    {
      giop_connection_unref(server->connection);
      server->connection=NULL;
      server->active_profile = NULL; /* mila: perhaps this is wrong: no connection means no profile */
    }
  return cnx != 0;
}

static ActionReturnType
lookup_name (GHashTable* table, CosNaming_Name * n, TableEntry **found,
	     CORBA_Environment * ev)
{

  if (n->_length == 0)		/* no name given. */
    {
      THROW_InvalidName (ev);
      return result_exception;
    }

  syslog(LOG_DEBUG, "lookup for = '%s'", NamingName2String(n));
  
  *found = g_hash_table_lookup( table, n->_buffer );

  if (*found)
    {
      if (n->_length > 1)	/* in a lower-level context */
	{
	  syslog(LOG_DEBUG,"lookup_name: Returning result_lower_level\n");
	  return result_lower_level;	/* in current context */
	}
      syslog(LOG_DEBUG,"lookup_name: Returning result_ok\n");
      return result_ok;
    }
  else
    {
      if (n->_length > 1)   /* result should be a context, but is not found */
	{
	  THROW_NotFound (missing_node, n, ev);
	  return result_exception;      
	}
    }
  syslog(LOG_DEBUG,"lookup_name: result_not_found\n");
  return result_not_found;
}

static void
insert_name (GHashTable* table, CosNaming_NameComponent * name,
	     CosNaming_BindingType type,
	     CORBA_Object obj,
	     CORBA_Environment * ev)
{
  TableEntry *insert = g_new (TableEntry, 1);
  
  syslog(LOG_DEBUG,"Inserting name = '%s.%s'\n", name->id, name->kind);
  
  NameComponent__copy( &insert->name, name );
  insert->type = type;
  insert->object = CORBA_Object_duplicate (obj, ev);
  g_hash_table_insert( table, &insert->name, insert );
}

static void
remove_name (GHashTable* table, CosNaming_NameComponent * name, 
	     CORBA_Environment * ev)
{
  TableEntry *found = g_hash_table_lookup(table, name);
  g_assert( found );

  syslog(LOG_DEBUG,"Removing name   = '%s.%s'\n", found->name.id,
	 found->name.kind);
  g_hash_table_remove( table, name );
  CORBA_free (found->name.id);
  CORBA_free (found->name.kind);
  CORBA_Object_release (found->object, ev); 
  g_free (found);
}



static CORBA_Object
prepare_resending (TableEntry *entry, CosNaming_Name * old,
		   CosNaming_Name ** new_name)
{
  if( entry->type != CosNaming_ncontext )
    {
      return NULL;      
    }

  *new_name = CosNaming_Name__alloc ();
  Name__copy ( *new_name, old, 1 );

  /* FIXME: narrowing to CosNaming_NamingContext */
  return entry->object;
}

static void
do_re_bind_no_context (GHashTable *table,
		       CosNaming_Name * n,
		       CORBA_Object obj,
		       gboolean is_rebind,
		       gboolean is_context,
		       CORBA_Environment * ev)
{
  TableEntry* entry;
  ActionReturnType res = lookup_name (table, n, &entry, ev);

  if (res == result_exception)
    return;
  if (res == result_lower_level)
    {
      CosNaming_Name *new_name;
      CORBA_Object context = prepare_resending (entry, n, &new_name);

      if( !context )
	{
	  THROW_NotFound (not_context, n, ev);
	  return;
	}
      
      if (is_rebind)
	{
	  if (is_context)
	    {
	      CosNaming_NamingContext_rebind_context (context, new_name, 
						      obj, ev);
	    }	  
	  else
	    {
	      CosNaming_NamingContext_rebind (context, new_name, obj, ev);
	    }
	}
      else
	{
	  if (is_context)
	    {
	      CosNaming_NamingContext_bind_context (context, new_name, 
						    obj, ev);
	    }
	  else
	    {
	      CosNaming_NamingContext_bind (context, new_name, obj, ev);
	    }
	}
      CORBA_free (new_name);
      return;
    }

  if (res == result_not_found && is_rebind)
    {
      THROW_NotFound (missing_node, n, ev);
      return;
    }

  if (res == result_ok && !is_rebind)
    {
      THROW_AlreadyBound (ev);
      return;
    }
 
  if (is_rebind)
    {
      remove_name (table, &entry->name, ev);
    }

  insert_name (table, n->_buffer,
	       is_context ? CosNaming_ncontext : CosNaming_nobject, obj, ev);
}

void
impl_CosNaming_NamingContext_bind (impl_POA_CosNaming_NamingContext * servant,
		    CosNaming_Name * n,
		    CORBA_Object obj,
		    CORBA_Environment * ev)
{
  do_re_bind_no_context (servant->table, n, obj, FALSE, FALSE, ev);
}

void
impl_CosNaming_NamingContext_rebind (impl_POA_CosNaming_NamingContext * servant,
		      CosNaming_Name * n,
		      CORBA_Object obj,
		      CORBA_Environment * ev)
{
  do_re_bind_no_context (servant->table, n, obj, TRUE, FALSE, ev);
}

void
impl_CosNaming_NamingContext_bind_context (impl_POA_CosNaming_NamingContext * servant,
			    CosNaming_Name * n,
			    CosNaming_NamingContext nc,
			    CORBA_Environment * ev)
{
  do_re_bind_no_context (servant->table, n, nc, FALSE, TRUE, ev);
}

void
impl_CosNaming_NamingContext_rebind_context (impl_POA_CosNaming_NamingContext * servant,
			      CosNaming_Name * n,
			      CosNaming_NamingContext nc,
			      CORBA_Environment * ev)
{
  do_re_bind_no_context (servant->table, n, nc, TRUE, TRUE, ev);
}

CORBA_Object
impl_CosNaming_NamingContext_resolve (impl_POA_CosNaming_NamingContext * servant,
		       CosNaming_Name * n,
		       CORBA_Environment * ev)
{
  TableEntry *found;
  ActionReturnType res = lookup_name (servant->table, n, &found, ev);

  syslog(LOG_DEBUG, "NameContext_resolving '%s', res = %d\n", 
	 NamingName2String(n), res); 

  if (res == result_exception) 
    {
      return CORBA_OBJECT_NIL;
    }

  if (res == result_lower_level)
    {
      CosNaming_Name *new_name;
      CORBA_Object result;
      CORBA_Object context = prepare_resending (found, n, &new_name);

      if( !context )
	{
	  THROW_NotFound (not_context, n, ev);
	  return CORBA_OBJECT_NIL;
	}
      
      result = CosNaming_NamingContext_resolve (context, new_name, ev);
      CORBA_free (new_name);
      return result;
    }
  
  if (res == result_not_found)
    {
      THROW_NotFound (missing_node, n, ev);
      return CORBA_OBJECT_NIL;
    }

  if (found->type == CosNaming_ncontext || server_is_alive(found->object))
    {
      return CORBA_Object_duplicate (found->object, ev);
    }

  syslog(LOG_DEBUG, "Removing dead server '%s' from name service\n", 
	 NamingName2String(n));
  remove_name (servant->table, &found->name, ev);
  THROW_NotFound (missing_node, n, ev);
  return CORBA_OBJECT_NIL;
}

void
impl_CosNaming_NamingContext_unbind (impl_POA_CosNaming_NamingContext * servant,
		      CosNaming_Name * n,
		      CORBA_Environment * ev)
{
  TableEntry *found;
  ActionReturnType res = lookup_name (servant->table, n, &found, ev);

  if (res == result_exception)
    {
      return;
    }

  if (res == result_lower_level)
    {
      CosNaming_Name *new_name;
      CORBA_Object context = prepare_resending (found, n, &new_name);

      if( !context )
	{
	  THROW_NotFound (not_context, n, ev);
	  return;
	}

      CosNaming_NamingContext_unbind (context, new_name, ev);
      CORBA_free (new_name);
      return;
    }

  if (res == result_not_found)
    {
      THROW_NotFound (missing_node, n, ev);
      return;
    }

  remove_name (servant->table, &found->name, ev);
}

CORBA_Object
impl_CosNaming_NamingContext_new_context (impl_POA_CosNaming_NamingContext * servant,
			   CORBA_Environment * ev)
{
  return impl_CosNaming_NamingContext__create (servant->poa, ev);
}

CORBA_Object
impl_CosNaming_NamingContext_bind_new_context (impl_POA_CosNaming_NamingContext * servant,
				CosNaming_Name * n,
				CORBA_Environment * ev)
{
  CosNaming_NamingContext context = 
    impl_CosNaming_NamingContext__create (servant->poa, ev);
  RETHROW (ev, NULL);
  do_re_bind_no_context (servant->table, n, context, FALSE, TRUE, ev);
  if ((ev)->_major != CORBA_NO_EXCEPTION )
    {
      impl_CosNaming_NamingContext_unbind ( servant, n, ev);
      CosNaming_NamingContext_destroy( context, ev );
    }
  return context;
}

void
impl_CosNaming_NamingContext_destroy (impl_POA_CosNaming_NamingContext * servant,
		       CORBA_Environment * ev)
{
  if (g_hash_table_size( servant->table ) > 0)
    {
      THROW_NotEmpty (ev);
      return;
    }
  g_hash_table_destroy(servant->table);
  impl_CosNaming_NamingContext__destroy( servant, ev );
  return;
}

void  
TableEntry_to_Binding(gpointer key /* not needed */,  
		      gpointer value,
		      gpointer user_data)
{
  CosNaming_Binding** curr_pointer = user_data;
  TableEntry* entry = value;

  (**curr_pointer).binding_name._length = 1;
  (**curr_pointer).binding_name._buffer = 
    CORBA_sequence_CosNaming_NameComponent_allocbuf (1);

  NameComponent__copy( (**curr_pointer).binding_name._buffer, &entry->name);

  (**curr_pointer).binding_type = entry->type;

  (*curr_pointer)++;
}

GArray*
HashTable_to_Array( GHashTable* table )
{
  int len = g_hash_table_size( table );
  void *curr_pointer;
  GArray* result = g_array_new( FALSE, FALSE, sizeof(CosNaming_Binding) );

  g_array_set_size( result, len );
  curr_pointer = result->data;

  g_hash_table_foreach( table, TableEntry_to_Binding, &curr_pointer);
 
  g_assert( curr_pointer == result->data + len * sizeof(CosNaming_Binding) );
    
  return result;
}

CosNaming_BindingList *
Array_to_BindingList( GArray* array, guint begin_pos, guint len )
{
  CosNaming_BindingList *result = CosNaming_BindingList__alloc ();
  g_assert( begin_pos + len <= array->len );
  result->_length = len;
  result->_buffer = CORBA_sequence_CosNaming_Binding_allocbuf (len);
  memcpy(result->_buffer, array->data + sizeof(CosNaming_Binding) * begin_pos, 
	 sizeof(CosNaming_Binding) * len );
  return result;
}

void
impl_CosNaming_NamingContext_list (impl_POA_CosNaming_NamingContext * servant,
		    CORBA_unsigned_long how_many,
		    CosNaming_BindingList ** bl,
		    CosNaming_BindingIterator * bi,
		    CORBA_Environment * ev)
{
  GArray* array = HashTable_to_Array( servant->table );

  if( array->len <= how_many )
    {
      *bi = CORBA_OBJECT_NIL;
      how_many = array->len;
    }
  else
    {
      impl_POA_CosNaming_BindingIterator * iterator_servant;
      *bi = impl_CosNaming_BindingIterator__create (servant->poa, 
						    &iterator_servant, ev);
      RETHROW_VOID(ev);
      iterator_servant->array = array; 
      iterator_servant->transmitted = how_many;
    }
  *bl = Array_to_BindingList( array, 0, how_many );
}

CORBA_boolean
impl_CosNaming_BindingIterator_next_one (impl_POA_CosNaming_BindingIterator * servant,
					 CosNaming_Binding ** b,
					 CORBA_Environment * ev)
{
  if(servant->transmitted < servant->array->len)
    {
      *b = CosNaming_Binding__alloc();
      memcpy( *b, servant->array->data + sizeof( CosNaming_Binding ) * 
	      servant->transmitted , sizeof( CosNaming_Binding ) );
      servant->transmitted++;
      return CORBA_TRUE;
    } 
  return CORBA_FALSE;
}

CORBA_boolean
impl_CosNaming_BindingIterator_next_n (impl_POA_CosNaming_BindingIterator * servant,
				       CORBA_unsigned_long how_many,
				       CosNaming_BindingList ** bl,
				       CORBA_Environment * ev)
{
  int len = MIN( how_many, servant->array->len - servant->transmitted);
  *bl = Array_to_BindingList( servant->array, servant->transmitted, len );
  servant->transmitted += len;
  
  return servant->array->len - servant->transmitted > 0 ? 
    CORBA_TRUE : CORBA_FALSE;
}

void
impl_CosNaming_BindingIterator_destroy (impl_POA_CosNaming_BindingIterator * servant,
					CORBA_Environment * ev)
{
  g_array_free (servant->array, TRUE);
  impl_CosNaming_BindingIterator__destroy( servant, ev );
  return;
}

void
signal_handler(int signo)
{
  syslog(LOG_ERR,"Receveived signal %d\nshutting down.", signo);
  switch(signo) {
    case SIGSEGV:
	abort();
    default:
	exit(1);
  }
}

int
main (int argc, char *argv[])
{
  CORBA_ORB orb;
  CORBA_Environment ev;
  CosNaming_NamingContext context;
  CORBA_char *objref;
  PortableServer_POA root_poa;
  PortableServer_POAManager pm;
  struct sigaction act;
  sigset_t         empty_mask;
  
  openlog("orbit-name-server", LOG_NDELAY | LOG_PID, LOG_DAEMON);
  syslog(LOG_INFO,"starting");
  sigemptyset(&empty_mask);
  act.sa_handler = signal_handler;
  act.sa_mask    = empty_mask;
  act.sa_flags   = 0;
  
  sigaction(SIGINT,  &act, 0);
  sigaction(SIGHUP,  &act, 0);
  sigaction(SIGSEGV, &act, 0);
  sigaction(SIGABRT, &act, 0);
  
  act.sa_handler = SIG_IGN;
  sigaction(SIGPIPE, &act, 0);
  
  CORBA_exception_init (&ev);
  
  orb = CORBA_ORB_init (&argc, argv, "orbit-local-orb", &ev);

  root_poa = (PortableServer_POA)
    CORBA_ORB_resolve_initial_references (orb, "RootPOA", &ev);

  context = impl_CosNaming_NamingContext__create(root_poa, &ev);

  objref = CORBA_ORB_object_to_string (orb, context, &ev);

  g_print ("%s\n", objref);
  fflush (stdout);

  CORBA_free (objref);

#if 0
  /* Don't release it so that the same-address-space info will stay around */
  CORBA_Object_release (context, &ev);
#endif

  pm = PortableServer_POA__get_the_POAManager (root_poa, &ev);
  PortableServer_POAManager_activate (pm, &ev);

  CORBA_ORB_run (orb, &ev);
  syslog(LOG_INFO, "exiting");
  return 0;
}
