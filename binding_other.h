#include "Python.h"
#include "../OpenStorm/Source/OpenStorm/Radar/Nexrad.h"

#include <string>

static PyObject* recompressNexradArchive(PyObject* self, PyObject* args) {
	// input file
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	// output file
	PyObject* arg1=PyTuple_GetItem(args, 1);
	if (arg1==NULL) return NULL;
	int result = Nexrad::RecompressArchive(std::string(PyUnicode_AsUTF8(arg0)), std::string(PyUnicode_AsUTF8(arg1)));
	return PyBool_FromLong(result == 0);
}