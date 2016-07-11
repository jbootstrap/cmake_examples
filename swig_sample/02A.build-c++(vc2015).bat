SETLOCAL

set SCRIPT=%~0
for /f "delims=\ tokens=*" %%z in ("%SCRIPT%") do (
  set SCRIPT_DRIVE=%%~dz
  set SCRIPT_PATH=%%~pz
  set SCRIPT_CURRENT_DIR=%%~dpz
)

cd /d %SCRIPT_CURRENT_DIR%
rem cd c++
rmdir /s /q cmake_32bit
mkdir cmake_32bit
cd cmake_32bit

cmake -G "Visual Studio 14 2015" -DCMAKE_CXX_FLAGS_RELEASE="/MT" -DCMAKE_CXX_FLAGS_DEBUG="/MTd" ^
                                 ..\c++
cmake --build . --config Release

mkdir %SCRIPT_CURRENT_DIR%java\native\x86
copy Release\*.dll %SCRIPT_CURRENT_DIR%java\native\x86\

cd /d %SCRIPT_CURRENT_DIR%

ENDLOCAL
PAUSE
