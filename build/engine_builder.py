import fnmatch
import os, sys, shutil, subprocess, platform
from distutils.dir_util import copy_tree
from test_runner import TestRunner
from build_config import *
from build_common import *


paths = PathManager()

class Builder:
	Threads = 9

	def __init__(self):
		self.clean_compile_directory = False
		self.build_samples = True
		self.use_jom = False
		self.use_nmake = False
		self.__ParseArgs()

	def __ParseArgs(self):
		print(BuildMessage.ArgumentsMsg)
		print(len(sys.argv))
		
		for i in range(1, len(sys.argv)):
			print(sys.argv[i])
			
			if sys.argv[i] == "-clean":
				self.clean_compile_directory = True
			elif sys.argv[i] == "-nosamples":
				self.build_samples = False
			elif sys.argv[i] == "-usejom":
				self.use_jom = True
			elif sys.argv[i] == "-nmake":
				self.use_nmake = True

		print(BuildMessage.ArgumentsMsg)

	def CleanCompileDir(self, dir):
		try:
			shutil.rmtree(dir)
		except:
			pass

	def CreateAndChDir(self, dir):
		if self.clean_compile_directory:
			self.CleanCompileDir(dir)

		if os.path.exists(dir) == False:
			try:
				os.mkdir(dir)
			except OSError as e:
				print(BuildMessage.FailedToCreateDirectory.format(dir))
				raise

		os.chdir(dir)
		return True

	@PreserveDirectory(paths.Paths['cwd'])
	def Compile(self):
		self.CreateAndChDir(paths.Paths['build'])
		self.CreateAndChDir(paths.Paths['lib'])

		os.chdir(paths.CMakePaths['glxw'])
		subprocess.check_call('python glxw_gen.py', shell=True)

		for key, value in paths.CMakePaths.items():
			if self.build_samples == False and key == "examples":
				continue

			self.CreateAndChDir(join(paths.Paths['build'], key))

			subprocess.check_call(self.GetCmakeCommand(key, value), shell=True)
			subprocess.check_call(self.GetCompileCommand(), shell=True)

			if key == "examples":
				self.CopyExampleResources()


			self.CopyLibs()

	def GetCmakeCommand(self, key, value):
		OS = platform.system()

		command = 'cmake "' + value + '"' 

		if key == "physfs":
			command += ' -DCXXFLAGS="Wno-unused-variable" -DPHYSFS_BUILD_TEST="FALSE"'

		command += ' -DENGINE_PATH:PATH="' + paths.Paths['engine'] + '"'

		if OS == "Windows":
			if self.use_jom or self.use_nmake:
				command += ' -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "NMake Makefiles"'
			else:
				command += ' -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "MinGW Makefiles"'
		else:
			command += ' -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "Unix Makefiles"'

		print("Cmake command: " + command)

		return command

	def GetCompileCommand(self):
		OS = platform.system()

		command = ''

		if OS == "Windows":
			if self.use_jom:
				command += 'jom.exe -j' + str(Builder.Threads)
			if self.use_nmake:
				command += 'nmake'
			else:
				command += 'mingw32-make.exe -j' + str(Builder.Threads)
		else:
			command += 'make -j' + str(Builder.Threads)

		return command


	def GetLibs(self, dir):
		match = []
		extension = ""

		if platform.system() == "Windows" and (self.use_jom or self.use_nmake):
			extension = ".lib"
		else:
			extension = ".a"

		for root, dirnames, filenames in os.walk(dir):
			for filename in fnmatch.filter(filenames, '*' + extension):
				if filename != "objects" + extension:
					match.append([filename,os.path.join(root, filename)])
						
		return match

	def CopyLibs(self):
		matches = []
		matches.extend(self.GetLibs(os.getcwd()))

		for f in matches:
			moved_file_path = os.path.join(paths.Paths["lib"], f[0])
			try:
				shutil.move(f[1], moved_file_path)
			except:
				print(BuildMessage.FailedToMoveFile.format(f[1], moved_file_path))
				raise

	def CopyExampleResources(self):
		copy_tree(paths.Paths["resources"], paths.Paths["example_bin"])

