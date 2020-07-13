/************************************************************************/
/* this file implement a python wrapper for asteroid float value		*/
/************************************************************************/

#pragma once

#if WITH_EDITOR

#include "UEPyModule.h"
#include "Channel/AsteroidFloatChannel.h"

/** define the float value structure */
typedef struct
{
	PyObject_HEAD
		/** type specific fields go here */
		FAsteroidFloatValue value;
} ue_PyFAsteroidFloatValue;

//	construct the structure
PyObject* py_ue_new_fasteroid_float_value(FAsteroidFloatValue);

//	check if the python object is asteroid float value
ue_PyFAsteroidFloatValue* py_ue_is_fasteroid_float_value(PyObject*);

//	register the structure to module
void ue_python_init_fasteroid_float_value(PyObject*);

#endif