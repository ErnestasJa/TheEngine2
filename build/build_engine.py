import fnmatch
import os, sys, shutil, subprocess
from collections import OrderedDict
from test_runner import TestRunner

pdir = os.path.dirname
join = os.path.join

def preserve_directory(directory):
	def preserve_decorator(fn):
		def wrapped(*args, **kwargs):
			os.chdir(directory)
			ret = fn(*args, **kwargs)
			os.chdir(directory)
			return ret
		return wrapped
	return preserve_decorator

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
		self.should_clean_compile_dir = len(sys.argv) == 2 and sys.argv[1] == "-clean"

	def CleanCompileDir(self, dir):
		try:
			shutil.rmtree(dir)
		except:
			pass

	def CreateAndChDir(self, dir):
		if self.should_clean_compile_dir:
			self.CleanCompileDir(dir)

		if os.path.exists(dir) == False:
			try:
				os.mkdir(dir)
			except OSError as e:
				print('Failed creating build directory: "' + dir + '"')
				return False

		os.chdir(dir)
		return True

	@preserve_directory(paths.Paths['cwd'])
	def Compile(self):
		if not self.CreateAndChDir(paths.Paths['build']):
			raise Exception("Failed creatring dir.")

		if not self.CreateAndChDir(paths.Paths['lib']):
			raise Exception("Failed creatring dir.")

		for key, value in paths.CMakePaths.items():
			if not self.CreateAndChDir(join(paths.Paths['build'], key)):
				raise Exception("Failed creatring dir.")

			subprocess.check_call('cmake "' + value + '" -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "Unix Makefiles"', shell=True)
			subprocess.check_call('make -j' + str(Builder.Threads), shell=True)
			self.copy_libs()

	def get_libs_from_dir(self, dir):
		match = []

		for root, dirnames, filenames in os.walk(dir):
			for filename in fnmatch.filter(filenames, '*.a'):
				if filename != "objects.a":
					match.append([filename,os.path.join(root, filename)])
					
		return match

	def copy_libs(self):
		matches = []
		matches.extend(self.get_libs_from_dir(os.getcwd()))

		for f in matches:
			moved_file_path = os.path.join(paths.Paths["lib"], f[0])
			try:
				shutil.move(f[1], moved_file_path)
			except:
				print("Failed to move: '" + f[1] + "', to: '" + moved_file_path + "'")
				pass

Builder().Compile()
TestRunner().Run()