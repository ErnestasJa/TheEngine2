import fnmatch
import os, shutil, subprocess

class Os:
	OsDict = {}
	CurrentOs = None
	
	def __init__(self, name, path):
		self.name = name
		self.path = path

	@staticmethod
	def Get(name):
		return Os.OsDict[name]

	@staticmethod
	def Add(name, path):
		Os.OsDict[name] = Os(name, path)

	@staticmethod
	def Init():
		Os.Add("linux","linux")
		Os.Add("windows", "win")

		CurrentOs = Os.Get('linux')


pdir = os.path.dirname

Paths = {
	"cmake" : os.getcwd(),
	"lib" : pdir(pdir(os.getcwd())),
	"build" : "build"
}

def preserve_directory(directory):
	def preserve_decorator(fn):
		def wrapped(*args, **kwargs):
			os.chdir(directory)
			ret = fn(*args, **kwargs)
			os.chdir(directory)
			return ret
		return wrapped
	return preserve_decorator

class Builder:
	call = subprocess.call
	Threads = 9

	def __init__(self):
		return

	@preserve_directory(Paths['cmake'])
	def CleanCompileDir(self):
		shutil.rmtree(os.path.join(Paths['cmake'],Paths['build']))

	@preserve_directory(Paths['cmake'])
	def Compile(self):
		if os.path.exists(Paths['build']) == False:
			try:
				os.mkdir(Paths['build'])
			except OSError as e:	  
				print('Failed creating build directory.')
				return

		os.chdir(Paths['build'])

		Builder.call('cmake ../ -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "Unix Makefiles"', shell=True)
		Builder.call('make -j' + str(Builder.Threads), shell=True)

		self.copy_libs()

	def get_libs_from_dir(self, dir):
		match = []

		for root, dirnames, filenames in os.walk(dir):
			for filename in fnmatch.filter(filenames, '*.a'):
				if filename != "objects.a":
					match.append([filename,os.path.join(root, filename)])
					
		return match

	@preserve_directory(Paths['cmake'])
	def copy_libs(self):
		matches = []
		matches.extend(self.get_libs_from_dir(os.getcwd()))

		for f in matches:
			filename = os.path.join(Paths["lib"], f[0])
			try:
				shutil.move(f[1], filename)
			except:
				pass


Os.Init()

Builder().Compile()