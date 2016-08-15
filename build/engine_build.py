from build_tools.build_tools import *

#example
compiler = GetCompiler()
CMaker = CMakeCommand(compiler)

enginePath = PathBuilder(FileSystem.Cwd()).Parent()
buildPath = PathBuilder(enginePath.Join("build"))
binPath = buildPath.Join("bin")
libPath = PathBuilder(buildPath.Join("lib"))

#compile libs & engine
CMaker.Exec(enginePath.Join("third_party/physfs"),  binPath.Join("physfs"), defines = {'PHYSFS_BUILD_TEST':'"FALSE"'})
CMaker.Exec(enginePath.Join("third_party/glfw"),    binPath.Join("glfw"))
CMaker.Exec(enginePath.Join("third_party/glxw"),    binPath.Join("glxw"), threads = 1)
CMaker.Exec(buildPath,                              binPath.Join("engine"), defines = {'ENGINE_PATH':'"'+str(enginePath)+'"'})

#copy files
FileSystem.CopyFiles(FileSystem.GetFilesByExtension(str(binPath), compiler.lib_extension()), libPath)
FileSystem.CopyFolder(enginePath.Join("resources"), binPath.Join("examples/resources"))

#compile tests & examples
CMaker.Exec(enginePath.Join("test"),     binPath.Join("test"), defines = {'ENGINE_PATH':'"'+str(enginePath)+'"','gtest_disable_pthreads':'ON'})
CMaker.Exec(enginePath.Join("examples"), binPath.Join("examples"), defines = {'ENGINE_PATH':'"'+str(enginePath)+'"'})