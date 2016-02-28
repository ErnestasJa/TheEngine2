import fnmatch
import os, sys, shutil, subprocess
from collections import OrderedDict
from test_runner import TestRunner

pdir = os.path.dirname
join = os.path.join

def PreserveDirectory(directory):
	def PreserveDecorator(fn):
		def Wrapped(*args, **kwargs):
			os.chdir(directory)
			ret = fn(*args, **kwargs)
			os.chdir(directory)
			return ret
		return Wrapped
	return PreserveDecorator

class BuildMessage:
	ArgumentsMsg = "======= Arguments ======="
	FailedToCreateDirectory = 'Failed to create build directory: "{0}"'
	FailedToMoveFile = 'Failed to move file "{0}" to "{1}".'

class PathManager:
	def __init__(self):
		self.Paths = {}
		self.Paths["cwd"] = os.getcwd()
		self.Paths["engine"] = pdir(self.Paths["cwd"])
		self.Paths["build"] = join(self.Paths["engine"], "build/bin")
		self.Paths["lib"] = join(self.Paths["engine"], "build/lib")

		self.CMakePaths = OrderedDict()
		self.CMakePaths["physfs"] = join(self.Paths["engine"],"third_party/physfs")
		self.CMakePaths["glfw"] = join(self.Paths["engine"],"third_party/glfw")
		self.CMakePaths["engine"] = join(self.Paths["engine"],"build/linux")
		self.CMakePaths["test"] = join(self.Paths["engine"],"test")
		self.CMakePaths["examples"] = join(self.Paths["engine"],"examples")

paths = PathManager()

class Builder:
	Threads = 9

	def __init__(self):
		self.clean_compile_directory = False
		self.build_samples = True
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

		for key, value in paths.CMakePaths.items():
			if self.build_samples == False and key == "examples":
				continue

			self.CreateAndChDir(join(paths.Paths['build'], key))
			subprocess.check_call('cmake "' + value + '" -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "Unix Makefiles"', shell=True)
			subprocess.check_call('make -j' + str(Builder.Threads), shell=True)
			self.CopyLibs()

	def GetLibs(self, dir):
		match = []

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

Builder().Compile()
TestRunner().Run()
