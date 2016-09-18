import os
import ycm_core

flags = [
  '-Wall',
  '-Wextra',
  '-Werror',
  '-Wno-long-long',
  '-Wno-variadic-macros',
  '-fexceptions',
  '-DNDEBUG',
  '-DUSE_CLANG_COMPLETER',
  '-std=c++14',
  '-x',
  'c++',
  
  # includes for project
  '-I', '.',
  '-I', '../src',
  '-I', '../include',
  '-I', '../third_party',
  '-I', '../third_party/glm',
  '-include', '../include/EngineInc.h'
]
  

SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ]

def DirectoryOfThisScript():
  return os.path.dirname( os.path.abspath( __file__ ) )

def FlagsDir(): 
  return os.path.join(DirectoryOfThisScript(), "flags")

def MakeRelativePathsInFlagsAbsolute( flags, working_directory ):
  if not working_directory:
    return list( flags )
  new_flags = []
  make_next_absolute = False
  path_flags = [ '-isystem', '-I', '-include', '-iquote', '--sysroot=' ]
  for flag in flags:
    new_flag = flag

    if make_next_absolute:
      make_next_absolute = False
      if not flag.startswith( '/' ):
        new_flag = os.path.abspath( os.path.join( working_directory, flag ) )

    for path_flag in path_flags:
      if flag == path_flag:
        make_next_absolute = True
        break

      if flag.startswith( path_flag ):
        path = flag[ len( path_flag ): ]
        new_flag = os.path.abs_path( path_flag + os.path.join( working_directory, path ) )
        break

    if new_flag:
      new_flags.append( new_flag )
  return new_flags

def MkDir():
  try:
    os.mkdir(FlagsDir())
  except:
    if not os.path.exists(FlagsDir()):
      raise

def FlagsForFile( filename, **kwargs ):
  relative_to = DirectoryOfThisScript()
  final_flags = MakeRelativePathsInFlagsAbsolute( flags, relative_to )

  try:
    MkDir()

    basename = os.path.basename(filename)
    flagfilename = os.path.join(FlagsDir(), basename+".flags")

    with open(flagfilename, 'w') as f:
      f.write(str(final_flags))

  except:
    pass

  return {
    'flags': final_flags,
    'do_cache': False
  }