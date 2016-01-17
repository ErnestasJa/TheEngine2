import fnmatch
import os, shutil, subprocess, stat
from collections import OrderedDict

pdir = os.path.dirname
join = os.path.join
executable = stat.S_IEXEC | stat.S_IXGRP | stat.S_IXOTH

class TestRunner:
	def __init__(self):
		pass

	def InitTestData(self):
		try:
			shutil.copytree("../test/testdata", "./bin/test/testdata")
		except FileExistsError:
			pass

		os.chdir('./bin/test')
	
	def Run(self):
		self.InitTestData()

		for filename in os.listdir('.'):
			if os.path.isfile(filename):
				st = os.stat(filename)
				mode = st.st_mode
				if mode & executable:
					print("\n\n\n================ RUNNING TEST: '" + filename + "' =======================\n\n\n")
					subprocess.check_call("./" + filename, shell=True)