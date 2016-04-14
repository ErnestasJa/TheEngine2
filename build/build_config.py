import os
import platform
from collections import OrderedDict

pdir = os.path.dirname
join = os.path.join

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
		self.CMakePaths["glxw"] = join(self.Paths["engine"],"third_party/glxw")
		self.CMakePaths["engine"] = join(self.Paths["engine"],"build/linux")
		if platform.system() != "Windows":
			self.CMakePaths["test"] = join(self.Paths["engine"],"test")
		self.CMakePaths["examples"] = join(self.Paths["engine"],"examples")