@echo off
cd ..
cd ..
SET ENGINE_PATH=%cd%
cd build
SET BUILD_PATH=%cd%
mkdir lib
cd lib
SET LIB_PATH=%cd%
cd ..
rmdir workspace
mkdir workspace
cd workspace
cmake %BUILD_PATH% -DBUILD_DIR="%BUILD_PATH%" -DENGINE_PATH="%ENGINE_PATH%" -G "Visual Studio 14 2015 Win64"