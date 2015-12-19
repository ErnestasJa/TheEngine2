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
	Threads = 9

	def __init__(self):
		return

	@preserve_directory(Paths['cmake'])
	def CleanCompileDir(self):
		shutil.rmtree(os.path.join(Paths['cmake'],Paths['build']))

	def CreateBuildDir(self):
		#if os.path.exists(Paths['build']):
		#	self.CleanCompileDir()

		try:
			os.mkdir(Paths['build'])
		except OSError as e:	  
			print('Failed creating build directory.')
			return


	@preserve_directory(Paths['cmake'])
	def Compile(self):
		self.CreateBuildDir()
		os.chdir(Paths['build'])
		subprocess.check_call('cmake ../ -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "Unix Makefiles"', shell=True)
		subprocess.check_call('make -j' + str(Builder.Threads), shell=True)
		subprocess.check_call('./PathTest')
#run
Os.Init()
Builder().Compile()
