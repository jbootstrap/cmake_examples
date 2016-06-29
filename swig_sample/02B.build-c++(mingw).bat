set SCRIPT=%~0
for /f "delims=\ tokens=*" %%z in ("%SCRIPT%") do (
  set SCRIPT_DRIVE=%%~dz
  set SCRIPT_PATH=%%~pz
  set SCRIPT_CURRENT_DIR=%%~dpz
)

SETLOCAL

set PATH=%PATH%;C:\mingw-w64\i686-5.3.0-posix-dwarf-rt_v4-rev0\mingw32\bin

cd /d %SCRIPT_CURRENT_DIR%
cd c++
rmdir /s /q cmake_mingw
mkdir cmake_mingw
cd cmake_mingw
cmake -G "MinGW Makefiles" -DCMAKE_SHARED_LINKER_FLAGS="-static" ..

cmake --build . --config Release

copy libmydll.dll %SCRIPT_CURRENT_DIR%mydll.dll

cd /d %SCRIPT_CURRENT_DIR%

ENDLOCAL
PAUSE
