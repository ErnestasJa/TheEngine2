try:
    from build_tools.build_tools import *
    import argparse
except ImportError as e:
    print("You need to fetch build_tools submodule before running this script")

# build arguments
parser = argparse.ArgumentParser()
parser.add_argument("-compiler", default="", required = False, choices=["gcc", "jom", "nmake", "mingw", "vs15"])
parser.add_argument("-noexamples", action='store_true', help="use this to skip examples build")
parser.add_argument("-notests", action='store_true', help="use this to skip tests")
parser.add_argument("-clean", action='store_true', help="use this to delete bin directory before building")
args = parser.parse_args()
print(args)

# setup

enginePath = PathBuilder(FileSystem.Cwd()).Parent()
buildPath = PathBuilder(enginePath.Join("build"))
binPath = buildPath.Join("bin")
libPath = PathBuilder(buildPath.Join("lib"))


compiler = GetCompiler(compiler = args.compiler)
CMaker = CMakeCommand(compiler, defines={'BUILD_DIR':str(binPath)})

if args.clean:
    FileSystem.RemoveDir(str(binPath))

# compile libs & engine
CMaker.Exec(enginePath.Join("third_party/physfs"),  	binPath.Join("physfs"),
    defines = {
        'PHYSFS_BUILD_TEST':'"FALSE"',
        'PHYSFS_BUILD_SHARED':'"FALSE"',
        'PHYSFS_BUILD_STATIC':'"TRUE"'
    })

CMaker.Exec(enginePath.Join("third_party/glfw"),    	binPath.Join("glfw"),
    defines = {
        'GLFW_BUILD_EXAMPLES':'OFF',
        'GLFW_BUILD_TESTS':'OFF',
        'GLFW_BUILD_DOCS':'OFF',
        'GLFW_INSTALL':'OFF'
    })

CMaker.Exec(enginePath.Join("third_party/glad"), 		binPath.Join("glad"))

CMaker.Exec(buildPath,                              	binPath.Join("engine"),
    defines = {
        'ENGINE_PATH':'"'+str(enginePath)+'"'
    })


# copy files
FileSystem.CopyFiles(FileSystem.GetFilesByExtension(str(binPath), compiler.lib_extension()), libPath)

# compile tests & examples
if not args.notests:
    CMaker.Exec(enginePath.Join("test"),     binPath.Join("test"), threads = 1,
        defines = {
            'ENGINE_PATH':'"'+str(enginePath)+'"',
            'gtest_disable_pthreads':'ON'
        })

if not args.noexamples:
    FileSystem.CopyFolder(enginePath.Join("resources"), binPath.Join("examples/resources"))
    CMaker.Exec(enginePath.Join("examples"), binPath.Join("examples"),
        defines = {
            'ENGINE_PATH':'"'+str(enginePath)+'"'
        })
