/************************************************************************/
/* this file implement a python wrapper for asteroid tangent data		*/
/************************************************************************/

#pragma once

#if WITH_EDITOR

#include "UEPyModule.h"
#include "Channel/AsteroidFloatChannel.h"

/** define the float value structure */
typedef struct
{
	PyObject_HEAD
		/** type specific filed */
		FAsteroidTangentData data;
} ue_PyFAsteroidTangentData;

//	construct
PyObject* py_ue_new_fasteroid_tangent_data(FAsteroidTangentData);

//	check
ue_PyFAsteroidTangentData* py_ue_is_fasteroid_tangent_data(PyObject*);

//	register
void ue_python_init_fasteroid_tangent_data(PyObject*);


#endif