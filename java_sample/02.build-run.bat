SETLOCAL

set SCRIPT=%~0
for /f "delims=\ tokens=*" %%z in ("%SCRIPT%") do (
  set SCRIPT_DRIVE=%%~dz
  set SCRIPT_PATH=%%~pz
  set SCRIPT_CURRENT_DIR=%%~dpz
)

cd /d %SCRIPT_CURRENT_DIR%
cd cmake_java

cmake --build . --config Release

ctest -C Release -V

ENDLOCAL
PAUSE
