from distutils.core import setup, Extension
import os
import glob

file_directory=os.path.dirname(__file__)
if file_directory:
    os.chdir(file_directory)

sources = ['binding.cpp']
depends = glob.glob("*.h")
#sources = list(map(lambda x: os.path.join(os.path.dirname(__file__), x), sources))

for root, dirs, files in os.walk("../OpenStorm/Source/OpenStorm/Radar/"):
	for file in files:
		file_path = os.path.join(root, file)
		if os.name == 'nt' and ("Deps/hdf5" in file_path or "Deps\\hdf5" in file_path):
			# exclude hdf5 on windows, because it blows past the 32767 character limit on command lines for the linker
			# damn you windows...
			continue
		if file.endswith(".cpp"):
			#print(file_path)
			sources.append(file_path)
		if file.endswith(".c"):
			sources.append(file_path)
		if file.endswith(".h"):
			depends.append(file_path)


mymodule = Extension('openstorm_radar_native', sources, depends=depends)

setup(
	name        = 'openstorm_radar_native',
	version     = '1.0',
	description = 'Manipulate radar data using OpenStorm',
	ext_modules = [mymodule]
)