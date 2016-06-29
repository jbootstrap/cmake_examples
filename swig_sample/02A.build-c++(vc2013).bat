set SCRIPT=%~0
for /f "delims=\ tokens=*" %%z in ("%SCRIPT%") do (
  set SCRIPT_DRIVE=%%~dz
  set SCRIPT_PATH=%%~pz
  set SCRIPT_CURRENT_DIR=%%~dpz
)

SETLOCAL

cd /d %SCRIPT_CURRENT_DIR%
cd c++
rmdir /s /q cmake_vc2013
mkdir cmake_vc2013
cd cmake_vc2013

cmake -G "Visual Studio 12 2013" -DCMAKE_CXX_FLAGS_RELEASE="/MT" -DCMAKE_CXX_FLAGS_DEBUG="/MTd" ^
                                 ..
cmake --build . --config Release

copy Release\*.dll %SCRIPT_CURRENT_DIR%

cd /d %SCRIPT_CURRENT_DIR%

ENDLOCAL
PAUSE
