import fnmatch
import os, shutil, subprocess

pdir = os.path.dirname

Paths = {
	"cwd" : os.getcwd(),
	"engine" : os.path.join(os.getcwd(), "linux")
	"lib" : pdir(os.getcwd()),
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
		pass

	@preserve_directory(Paths['cwd'])
	def CleanCompileDir(self):
		shutil.rmtree(os.path.join(Paths['cwd'],Paths['build']))

	@preserve_directory(Paths['cwd'])
	def Compile(self):
		if os.path.exists(Paths['build']) == False:
			try:
				os.mkdir(Paths['build'])
			except OSError as e:
				print('Failed creating build directory.')
				return

		os.chdir(Paths['build'])

		subprocess.check_call('cmake ../ -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "Unix Makefiles"', shell=True)
		subprocess.check_call('make -j' + str(Builder.Threads), shell=True)

		self.copy_libs()

	def get_libs_from_dir(self, dir):
		match = []

		for root, dirnames, filenames in os.walk(dir):
			for filename in fnmatch.filter(filenames, '*.a'):
				if filename != "objects.a":
					match.append([filename,os.path.join(root, filename)])
					
		return match

	@preserve_directory(Paths['cwd'])
	def copy_libs(self):
		matches = []
		matches.extend(self.get_libs_from_dir(os.getcwd()))

		for f in matches:
			filename = os.path.join(Paths["lib"], f[0])
			try:
				shutil.move(f[1], filename)
			except:
				pass

Builder().Compile()
