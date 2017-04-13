setlocal
set PATH=%PATH%;%cd%
call "%ProgramFiles(x86)%\\Microsoft Visual Studio\\2017\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" amd64
cd ..
python build_engine.py -compiler jom -clean -notests -noexamples
pause