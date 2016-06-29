SETLOCAL

set SCRIPT=%~0
for /f "delims=\ tokens=*" %%z in ("%SCRIPT%") do (
  set SCRIPT_DRIVE=%%~dz
  set SCRIPT_PATH=%%~pz
  set SCRIPT_CURRENT_DIR=%%~dpz
)


set PATH=%PATH%;C:\mingw-w64\i686-5.3.0-posix-dwarf-rt_v4-rev0\mingw32\bin

cd /d %SCRIPT_CURRENT_DIR%
rmdir /s /q cmake_extra
mkdir cmake_extra
cd cmake_extra
cmake -G "MinGW Makefiles" ..\java
cmake --build . --config Release

copy %SCRIPT_CURRENT_DIR%mydll.dll .
ctest -C Release -V

ENDLOCAL
PAUSE
