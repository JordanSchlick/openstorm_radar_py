#include "Python.h"
#include "../OpenStorm/Source/OpenStorm/Radar/RadarDataHolder.h"

#include <string>

static PyObject* radarDataHolderAllocate(PyObject* self, PyObject* args) {
	RadarDataHolder* radarDataHolder = new RadarDataHolder();
	return PyLong_FromVoidPtr(radarDataHolder);
}

static PyObject* radarDataHolderDeallocate(PyObject* self, PyObject* args) {
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL)
		return NULL;
	delete (RadarDataHolder*)PyLong_AsVoidPtr(arg0);
	return PyBool_FromLong(1);
}



static PyObject* radarDataHolderLoad(PyObject* self, PyObject* args) {
	// radar data holder
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	// file name (optional)
	PyObject* arg1=PyTuple_GetItem(args, 1);
	RadarDataHolder* radarDataHolder = (RadarDataHolder*)PyLong_AsVoidPtr(arg0);
	if(arg1 != NULL){
		RadarFile file = {};
		file.path = std::string(PyUnicode_AsUTF8(arg1));
		radarDataHolder->Load(file);
	}else{
		radarDataHolder->Load();
	}
	
	return PyBool_FromLong(1);
}

static PyObject* radarDataHolderUnload(PyObject* self, PyObject* args) {
	// radar data holder
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	RadarDataHolder* radarDataHolder = (RadarDataHolder*)PyLong_AsVoidPtr(arg0);
	radarDataHolder->Unload();
	return PyBool_FromLong(1);
}

static PyObject* radarDataHolderGetState(PyObject* self, PyObject* args) {
	// radar data holder
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	RadarDataHolder* radarDataHolder = (RadarDataHolder*)PyLong_AsVoidPtr(arg0);
	return PyLong_FromLong((int)radarDataHolder->state);
}

static PyObject* radarDataHolderGetProduct(PyObject* self, PyObject* args) {
	// radar data holder
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	// volume type
	PyObject* arg1=PyTuple_GetItem(args, 1);
	if (arg1==NULL) return NULL;
	RadarDataHolder* radarDataHolder = (RadarDataHolder*)PyLong_AsVoidPtr(arg0);
	RadarDataHolder::ProductHolder* radarDataHolderProduct = radarDataHolder->GetProduct((RadarData::VolumeType)PyLong_AsLong(arg1));
	radarDataHolderProduct->isFinal = true;
	return PyLong_FromVoidPtr(radarDataHolderProduct);
}

static PyObject* radarDataHolderProductGetRadarData(PyObject* self, PyObject* args) {
	// radar data holder product
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	RadarDataHolder::ProductHolder* radarDataHolderProduct = (RadarDataHolder::ProductHolder*)PyLong_AsVoidPtr(arg0);
	if(radarDataHolderProduct->radarData != NULL){
		radarDataHolderProduct->radarData->Decompress();
	}
	return PyLong_FromVoidPtr(radarDataHolderProduct->radarData);
}

static PyObject* radarDataHolderProductIsLoaded(PyObject* self, PyObject* args) {
	// radar data holder product
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	RadarDataHolder::ProductHolder* radarDataHolderProduct = (RadarDataHolder::ProductHolder*)PyLong_AsVoidPtr(arg0);
	return PyBool_FromLong(radarDataHolderProduct->isLoaded);
}

static PyObject* radarDataHolderProductStartUsing(PyObject* self, PyObject* args) {
	// radar data holder product
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	RadarDataHolder::ProductHolder* radarDataHolderProduct = (RadarDataHolder::ProductHolder*)PyLong_AsVoidPtr(arg0);
	radarDataHolderProduct->StartUsing();
	return PyBool_FromLong(1);
}

static PyObject* radarDataHolderProductStopUsing(PyObject* self, PyObject* args) {
	// radar data holder product
	PyObject* arg0=PyTuple_GetItem(args, 0);
	if (arg0==NULL) return NULL;
	RadarDataHolder::ProductHolder* radarDataHolderProduct = (RadarDataHolder::ProductHolder*)PyLong_AsVoidPtr(arg0);
	radarDataHolderProduct->StopUsing();
	return PyBool_FromLong(1);
}