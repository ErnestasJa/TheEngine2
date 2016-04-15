import fnmatch
import os, sys, shutil, subprocess, platform
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
			if platform.system() == "Windows":
				subprocess.check_call('cmake "' 
					+ value + '"'
					+ ' -DENGINE_PATH:PATH="' + paths.Paths['engine'] + '"' 
					+ ' -DWINDOWS_BUILD=1'
					+ ' -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "NMake Makefiles"', shell=True)
				if self.use_jom == True:
					subprocess.check_call('jom.exe -j' + str(Builder.Threads), shell=True)
				else:
					subprocess.check_call('nmake', shell=True)
			else:
				subprocess.check_call('cmake "' 
					+ value + '"'
					+ ' -DENGINE_PATH:PATH="' + paths.Paths['engine'] + '"' 
					+ ' -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "NMake Makefiles"', shell=True)
				subprocess.check_call('make -j' + str(Builder.Threads), shell=True)
			self.CopyLibs()

	def GetLibs(self, dir):
		match = []
		if platform.system() == "Windows":
			for root, dirnames, filenames in os.walk(dir):
				for filename in fnmatch.filter(filenames, '*.lib'):
					if filename != "objects.lib":
						match.append([filename,os.path.join(root, filename)])
		else:
			for root, dirnames, filenames in os.walk(dir):
				for filename in fnmatch.filter(filenames, '*.a'):
					if filename != "objects.a":
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
