import sys
import os
import numpy as np
import importlib.util
import ctypes
from enum import Enum

native_path = ""
if True:
	buildDir = os.path.join(os.path.dirname(__file__),'build')
	outputDirs = os.listdir(buildDir)
	#print(outputDirs)
	for dir in outputDirs:
		if dir.find("lib") == 0:
			#print(os.path.join(buildDir,dir))
			sys.path.insert(1, os.path.join(buildDir,dir))
			files = os.listdir(os.path.join(buildDir,dir))
			if native_path == "" or dir.find(str(sys.version_info.major) + "." + str(sys.version_info.minor)) > 0:
				native_path = os.path.join(buildDir,dir,files[0])
				
	#sys.path.insert(1, os.path.join(os.path.dirname(__file__),'build/lib.win-amd64-3.7'))

def module_from_file(module_name, file_path):
	spec = importlib.util.spec_from_file_location(module_name, file_path)
	module = importlib.util.module_from_spec(spec)
	spec.loader.exec_module(module)
	return module
native_module = module_from_file("openstorm_radar_native",native_path)
#print(nativeModule)
#import openstorm_radar_native as native_module

class VolumeTypes:
	VOLUME_UNKNOWN = 0
		
	# raw volume types
	
	VOLUME_REFLECTIVITY = 1
	VOLUME_VELOCITY = 2
	VOLUME_SPECTRUM_WIDTH = 3
	
	# computed volume types
	
	VOLUME_VELOCITY_DEALIASED = 102
	VOLUME_ROTATION = 103
	VOLUME_STORM_RELATIVE_VELOCITY = 104


class RadarData:
	"""
	stores radar data in a contiguous 3 dimensional buffer
	See https://github.com/JordanSchlick/OpenStorm/blob/main/Source/OpenStorm/Radar/RadarData.h for more info
	"""
	
	
	def __init__(self, max_sweeps=0, max_thetas=0, max_radius=0, existing_pointer=None, auto_free=True, product_holder=None):
		"""
		max_sweeps, max_thetas, and max_radius determine the size of the buffer used for the radar data
		if left as 0 the size will be automatically determined based off of the first radar file loaded
		"""
		self.auto_free = auto_free
		self.product_holder = product_holder
		if existing_pointer is None:
			self._ptr = native_module.radarDataAllocate(max_sweeps, max_thetas, max_radius)
		else:
			self._ptr = existing_pointer
		self._update_properties()
	
	def __del__(self):
		if self.auto_free:
			native_module.radarDataDeallocate(self._ptr)
	
	def load_nexrad_file(self, fileName, volume_type):
		"""load a nexrad file from disk into buffer"""
		native_module.radarDataLoadFile(self._ptr, fileName, volume_type)
		self._update_properties()
	
	
	def _update_properties(self):
		"""pull properties from native RadarData and make them accessible in python"""
		native_module.radarDataUpdateProperties(self._ptr, self)
	
	def get_stats(self):
		"""get info about the currently loaded radar volume"""
		return native_module.radarDataGetStats(self._ptr)




class RadarDataHolder:
	"""
	a class that holds the radar products and related information
	it also manages loading in radar files and products asynchronously
	"""
	DataStateUnloaded = 0
	DataStateLoading = 1
	DataStateLoaded = 2
	DataStateFailed = 3
	
	def __init__(self, existing_pointer=None, auto_free=True):
		self.auto_free = auto_free
		if existing_pointer is None:
			self._ptr = native_module.radarDataHolderAllocate()
		else:
			self._ptr = existing_pointer
	
	def __del__(self):
		if self.auto_free:
			native_module.radarDataHolderDeallocate(self._ptr)
	
	def load(self, file_name=None):
		"""
		load a file or load new products if file_name is not specified
		this will keep existing data so call unload if changing files
		"""
		if file_name is None:
			native_module.radarDataHolderLoad(self._ptr)
		else:
			native_module.radarDataHolderLoad(self._ptr, file_name)
	
	def unload(self, file_name=None):
		"""unload products and cancel loading"""
		native_module.radarDataHolderUnload(self._ptr)
	
	
	def get_product(self, volume_type):
		"""get a product to be loaded or add it if it is not found, should be called before Load if adding new product"""
		pointer = native_module.radarDataHolderGetProduct(self._ptr, volume_type)
		if pointer > 0:
			return RadarDataHolderProduct(pointer)
		else:
			return None
	
	
	def get_state(self):
		"""returns DataState enums"""
		return native_module.radarDataHolderGetState(self._ptr)


class RadarDataHolderProduct:
	"""
	holds info about a product and the radar data related to it
	"""
	def __init__(self, existing_pointer=None):
		self._ptr = existing_pointer
		native_module.radarDataHolderProductStartUsing(self._ptr)
		#print("radarDataHolderProductStartUsing")
	
	def __del__(self):
		native_module.radarDataHolderProductStopUsing(self._ptr)
		#print("radarDataHolderProductStopUsing")
	
	def get_radar_data(self):
		"""get the radar data for this product"""
		pointer = native_module.radarDataHolderProductGetRadarData(self._ptr)
		if pointer > 0:
			# radar data is managed by the holder so do not free it and pass self to prevent self from falling from scope which may take the native RadarData with it
			return RadarData(existing_pointer=pointer, product_holder=self, auto_free=False)
		else:
			return None
	
	def is_loaded(self):
		"""true if the radar data for this product is ready"""
		return native_module.radarDataHolderProductIsLoaded(self._ptr)
		
