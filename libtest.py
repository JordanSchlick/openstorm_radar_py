import sys
import os
import time
import numpy as np
sys.path.insert(0, os.path.join(os.path.dirname(__file__),'../'))
import openstorm_radar_py

try:
	os.chdir(os.path.dirname(__file__))
except:
	pass

print(openstorm_radar_py.native_path)
#print(openstorm_radar_py.native_module.test())

radar_data = openstorm_radar_py.RadarData()


print(radar_data._ptr)
radar_data.load_nexrad_file("../OpenStorm/Content/Data/Demo/KMKX_20220723_235820", openstorm_radar_py.VolumeTypes.VOLUME_REFLECTIVITY)

print(radar_data.get_stats())
# print(radar_data.test_prop)
print(radar_data.buffer)
print(np.array(radar_data.buffer)[0])
print(np.array(radar_data.buffer)[0,0,0],np.array(radar_data.buffer)[0,0,0],np.array(radar_data.buffer)[0,0,0],np.array(radar_data.buffer)[0,0,0],np.array(radar_data.buffer)[0,0,0])
print(np.array(radar_data.buffer).shape)


radar_data_holder = openstorm_radar_py.RadarDataHolder()
reflectivity_product = radar_data_holder.get_product(openstorm_radar_py.VolumeTypes.VOLUME_REFLECTIVITY)
rotation_product = radar_data_holder.get_product(openstorm_radar_py.VolumeTypes.VOLUME_ROTATION)
srv_product = radar_data_holder.get_product(openstorm_radar_py.VolumeTypes.VOLUME_STORM_RELATIVE_VELOCITY)
radar_data_holder.load("../OpenStorm/Content/Data/Demo/KTLX20130531_231434_V06.gz")
print(radar_data_holder.get_state())
while(radar_data_holder.get_state() == openstorm_radar_py.RadarDataHolder.DataStateLoading):
	print("loading...", end='\r')
	time.sleep(0.1)
print("loaded      ", end='\n')
print(radar_data_holder.get_state())
reflectivity_data = reflectivity_product.get_radar_data()
rotation_data = rotation_product.get_radar_data()
srv_data = srv_product.get_radar_data()

print("is reflectivity loaded?", reflectivity_product.is_loaded(), "   is rotation loaded?", rotation_product.is_loaded(), "   is srv loaded?", srv_product.is_loaded())

print(np.array(reflectivity_data.buffer)[0,:,:1000])
print(np.array(rotation_data.buffer)[0,:,:1000])
print(np.array(srv_data.buffer)[0,:,:1000])

print("finished")
