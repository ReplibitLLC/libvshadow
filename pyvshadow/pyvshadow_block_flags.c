/*
 * Python object definition of the libvshadow block flags
 *
 * Copyright (C) 2011-2014, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyvshadow_block_flags.h"
#include "pyvshadow_libvshadow.h"
#include "pyvshadow_python.h"
#include "pyvshadow_unused.h"

PyTypeObject pyvshadow_block_flags_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pyvshadow.block_flags",
	/* tp_basicsize */
	sizeof( pyvshadow_block_flags_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyvshadow_block_flags_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
        /* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pyvshadow block flags object (wraps LIBVSHADOW_BLOCK_FLAGS)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyvshadow_block_flags_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Initializes the type object
 * Returns 1 if successful or -1 on error
 */
int pyvshadow_block_flags_init_type(
     PyTypeObject *type_object )
{
	if( type_object == NULL )
	{
		return( -1 );
	}
	type_object->tp_dict = PyDict_New();

	if( type_object->tp_dict == NULL )
	{
		return( -1 );
	}
	if( PyDict_SetItemString(
             type_object->tp_dict,
             "IS_FORWARDER",
             PyInt_FromLong(
              LIBVSHADOW_BLOCK_FLAG_IS_FORWARDER ) ) != 0 )
	{
		goto on_error;
	}
	if( PyDict_SetItemString(
             type_object->tp_dict,
             "IS_OVERLAY",
             PyInt_FromLong(
              LIBVSHADOW_BLOCK_FLAG_IS_OVERLAY ) ) != 0 )
	{
		goto on_error;
	}
	if( PyDict_SetItemString(
             type_object->tp_dict,
             "NOT_USED",
             PyInt_FromLong(
              LIBVSHADOW_BLOCK_FLAG_NOT_USED ) ) != 0 )
	{
		goto on_error;
	}
	return( 1 );

on_error:
	if( type_object->tp_dict != NULL )
	{
		Py_DecRef(
		 type_object->tp_dict );

		type_object->tp_dict = NULL;
	}
	return( -1 );
}

/* Creates a new block flags object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvshadow_block_flags_new(
           void )
{
	pyvshadow_block_flags_t *pyvshadow_block_flags = NULL;
	static char *function                  = "pyvshadow_block_flags_new";

	pyvshadow_block_flags = PyObject_New(
	                     struct pyvshadow_block_flags,
	                     &pyvshadow_block_flags_type_object );

	if( pyvshadow_block_flags == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize block flags.",
		 function );

		goto on_error;
	}
	if( pyvshadow_block_flags_init(
	     pyvshadow_block_flags ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize block flags.",
		 function );

		goto on_error;
	}
	return( (PyObject *) pyvshadow_block_flags );

on_error:
	if( pyvshadow_block_flags != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyvshadow_block_flags );
	}
	return( NULL );
}

/* Intializes a block flags object
 * Returns 0 if successful or -1 on error
 */
int pyvshadow_block_flags_init(
     pyvshadow_block_flags_t *pyvshadow_block_flags )
{
	static char *function = "pyvshadow_block_flags_init";

	if( pyvshadow_block_flags == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid block flags.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Frees a block flags object
 */
void pyvshadow_block_flags_free(
      pyvshadow_block_flags_t *pyvshadow_block_flags )
{
	static char *function = "pyvshadow_block_flags_free";

	if( pyvshadow_block_flags == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid block flags.",
		 function );

		return;
	}
	if( pyvshadow_block_flags->ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid block flags - missing ob_type.",
		 function );

		return;
	}
	if( pyvshadow_block_flags->ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid block flags - invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	pyvshadow_block_flags->ob_type->tp_free(
	 (PyObject*) pyvshadow_block_flags );
}
