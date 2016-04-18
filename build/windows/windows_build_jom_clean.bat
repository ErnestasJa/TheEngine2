setlocal
set PATH=%PATH%;%cd%
call "%ProgramFiles(x86)%\\Microsoft Visual Studio 14.0\\VC\\vcvarsall.bat" amd64
cd ..
python build.py -usejom -clean
pause