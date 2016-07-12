set SCRIPT=%~0
for /f "delims=\ tokens=*" %%z in ("%SCRIPT%") do (
  set SCRIPT_DRIVE=%%~dz
  set SCRIPT_PATH=%%~pz
  set SCRIPT_CURRENT_DIR=%%~dpz
)

SETLOCAL

cd /d %SCRIPT_CURRENT_DIR%
rmdir /s /q cmake_32bit
mkdir cmake_32bit
cd cmake_32bit
cmake -G "Visual Studio 14 2015" ..

cmake --build . --config Release

ENDLOCAL

PAUSE
