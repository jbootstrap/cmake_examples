set SCRIPT=%~0
for /f "delims=\ tokens=*" %%z in ("%SCRIPT%") do (
  set SCRIPT_DRIVE=%%~dz
  set SCRIPT_PATH=%%~pz
  set SCRIPT_CURRENT_DIR=%%~dpz
)

SETLOCAL

cd /d %SCRIPT_CURRENT_DIR%
cd c++
rmdir /s /q cmake_vc2013_x64
mkdir cmake_vc2013_x64
cd cmake_vc2013_x64

cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_CXX_FLAGS_RELEASE="/MT" -DCMAKE_CXX_FLAGS_DEBUG="/MTd" ^
                                 ..
cmake --build . --config Release

rem copy Release\*.dll %SCRIPT_CURRENT_DIR%

mkdir %SCRIPT_CURRENT_DIR%java\native\x64
copy Release\*.dll %SCRIPT_CURRENT_DIR%java\native\x64\

cd /d %SCRIPT_CURRENT_DIR%

ENDLOCAL
PAUSE
