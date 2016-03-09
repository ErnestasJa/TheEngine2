import os

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