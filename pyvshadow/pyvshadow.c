/*
 * Python bindings module for libvshadow (pyvshadow)
 *
 * Copyright (C) 2011-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyvshadow.h"
#include "pyvshadow_block.h"
#include "pyvshadow_block_flags.h"
#include "pyvshadow_blocks.h"
#include "pyvshadow_error.h"
#include "pyvshadow_file_object_io_handle.h"
#include "pyvshadow_libcerror.h"
#include "pyvshadow_libvshadow.h"
#include "pyvshadow_python.h"
#include "pyvshadow_store.h"
#include "pyvshadow_stores.h"
#include "pyvshadow_unused.h"
#include "pyvshadow_volume.h"

#if !defined( LIBVSHADOW_HAVE_BFIO )

LIBVSHADOW_EXTERN \
int libvshadow_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libvshadow_error_t **error );

#endif /* !defined( LIBVSHADOW_HAVE_BFIO ) */

/* The pyvshadow module methods
 */
PyMethodDef pyvshadow_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pyvshadow_get_version,
	  METH_NOARGS,
	  "get_version() -> String\n"
	  "\n"
	  "Retrieves the version." },

	{ "check_volume_signature",
	  (PyCFunction) pyvshadow_check_volume_signature,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_volume_signature(filename) -> Boolean\n"
	  "\n"
	  "Checks if a volume has a Windows NT Volume Shadow Snapshot (VSS) volume signature." },

	{ "check_volume_signature_file_object",
	  (PyCFunction) pyvshadow_check_volume_signature_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_volume_signature(file_object) -> Boolean\n"
	  "\n"
	  "Checks if a volume has a Windows NT Volume Shadow Snapshot (VSS) volume signature using a file-like object." },

	{ "open",
	  (PyCFunction) pyvshadow_volume_new_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> Object\n"
	  "\n"
	  "Opens a volume." },

	{ "open_file_object",
	  (PyCFunction) pyvshadow_volume_new_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> Object\n"
	  "\n"
	  "Opens a volume using a file-like object." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

/* Retrieves the pyvshadow/libvshadow version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvshadow_get_version(
           PyObject *self PYVSHADOW_ATTRIBUTE_UNUSED,
           PyObject *arguments PYVSHADOW_ATTRIBUTE_UNUSED )
{
	const char *errors           = NULL;
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYVSHADOW_UNREFERENCED_PARAMETER( self )
	PYVSHADOW_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libvshadow_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         errors ) );
}

/* Checks if the volume has a Windows NT Volume Shadow Snapshot (VSS) volume signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvshadow_check_volume_signature(
           PyObject *self PYVSHADOW_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pyvshadow_check_volume_signature";
	static char *keyword_list[]  = { "filename", NULL };
	const char *filename_narrow  = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	PYVSHADOW_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyvshadow_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libvshadow_check_volume_signature_wide(
		          filename_wide,
		          &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyvshadow_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
				   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libvshadow_check_volume_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result == -1 )
		{
			pyvshadow_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check volume signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyvshadow_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   string_object );
#else
		filename_narrow = PyString_AsString(
				   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libvshadow_check_volume_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pyvshadow_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check volume signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Checks if the volume has a Windows NT Volume Shadow Snapshot (VSS) volume signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyvshadow_check_volume_signature_file_object(
           PyObject *self PYVSHADOW_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error         = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	PyObject *file_object            = NULL;
	static char *function            = "pyvshadow_check_volume_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	PYVSHADOW_UNREFERENCED_PARAMETER( self )

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &file_object ) == 0 )
	{
		return( NULL );
	}
	if( pyvshadow_file_object_initialize(
	     &file_io_handle,
	     file_object,
	     &error ) != 1 )
	{
		pyvshadow_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libvshadow_check_volume_signature_file_io_handle(
	          file_io_handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyvshadow_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to check volume signature.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     &error ) != 1 )
	{
		pyvshadow_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

#if PY_MAJOR_VERSION >= 3

/* The pyvshadow module definition
 */
PyModuleDef pyvshadow_module_definition = {
	PyModuleDef_HEAD_INIT,

	/* m_name */
	"pyvshadow",
	/* m_doc */
	"Python libvshadow module (pyvshadow).",
	/* m_size */
	-1,
	/* m_methods */
	pyvshadow_module_methods,
	/* m_reload */
	NULL,
	/* m_traverse */
	NULL,
	/* m_clear */
	NULL,
	/* m_free */
	NULL,
};

#endif /* PY_MAJOR_VERSION >= 3 */

/* Initializes the pyvshadow module
 */
#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_pyvshadow(
                void )
#else
PyMODINIT_FUNC initpyvshadow(
                void )
#endif
{
	PyObject *module                      = NULL;
	PyTypeObject *block_type_object       = NULL;
	PyTypeObject *block_flags_type_object = NULL;
	PyTypeObject *blocks_type_object      = NULL;
	PyTypeObject *store_type_object       = NULL;
	PyTypeObject *stores_type_object      = NULL;
	PyTypeObject *volume_type_object      = NULL;
	PyGILState_STATE gil_state            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libvshadow_notify_set_stream(
	 stderr,
	 NULL );
	libvshadow_notify_set_verbose(
	 1 );
#endif

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
#if PY_MAJOR_VERSION >= 3
	module = PyModule_Create(
	          &pyvshadow_module_definition );
#else
	module = Py_InitModule3(
	          "pyvshadow",
	          pyvshadow_module_methods,
	          "Python libvshadow module (pyvshadow)." );
#endif
	if( module == NULL )
	{
#if PY_MAJOR_VERSION >= 3
		return( NULL );
#else
		return;
#endif
	}
	PyEval_InitThreads();

	gil_state = PyGILState_Ensure();

	/* Setup the volume type object
	 */
	pyvshadow_volume_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyvshadow_volume_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyvshadow_volume_type_object );

	volume_type_object = &pyvshadow_volume_type_object;

	PyModule_AddObject(
	 module,
	 "volume",
	 (PyObject *) volume_type_object );

	/* Setup the stores type object
	 */
	pyvshadow_stores_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyvshadow_stores_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyvshadow_stores_type_object );

	stores_type_object = &pyvshadow_stores_type_object;

	PyModule_AddObject(
	 module,
	 "_stores",
	 (PyObject *) stores_type_object );

	/* Setup the store type object
	 */
	pyvshadow_store_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyvshadow_store_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyvshadow_store_type_object );

	store_type_object = &pyvshadow_store_type_object;

	PyModule_AddObject(
	 module,
	 "store",
	 (PyObject *) store_type_object );

	/* Setup the blocks type object
	 */
	pyvshadow_blocks_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyvshadow_blocks_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyvshadow_blocks_type_object );

	blocks_type_object = &pyvshadow_blocks_type_object;

	PyModule_AddObject(
	 module,
	 "_blocks",
	 (PyObject *) blocks_type_object );

	/* Setup the block type object
	 */
	pyvshadow_block_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyvshadow_block_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyvshadow_block_type_object );

	block_type_object = &pyvshadow_block_type_object;

	PyModule_AddObject(
	 module,
	 "block",
	 (PyObject *) block_type_object );

	/* Setup the block flags type object
	 */
	pyvshadow_block_flags_type_object.tp_new = PyType_GenericNew;

	if( pyvshadow_block_flags_init_type(
	     &pyvshadow_block_flags_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyvshadow_block_flags_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyvshadow_block_flags_type_object );

	block_flags_type_object = &pyvshadow_block_flags_type_object;

	PyModule_AddObject(
	 module,
	 "block_flags",
	 (PyObject *) block_flags_type_object );

	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( module );
#else
	return;
#endif

on_error:
	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( NULL );
#else
	return;
#endif
}

