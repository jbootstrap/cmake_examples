set SCRIPT=%~0
for /f "delims=\ tokens=*" %%z in ("%SCRIPT%") do (
  set SCRIPT_DRIVE=%%~dz
  set SCRIPT_PATH=%%~pz
  set SCRIPT_CURRENT_DIR=%%~dpz
)

SETLOCAL

cd /d %SCRIPT_CURRENT_DIR%
rmdir /s /q cmake_vc2015
mkdir cmake_vc2015
cd cmake_vc2015
cmake -G "Visual Studio 14 2015" -DCMAKE_CXX_FLAGS_RELEASE="/MT" -DCMAKE_CXX_FLAGS_DEBUG="/MTd" ^
                                 -DBOOST_LIBRARYDIR=E:\local\boost_1_61_0\lib32-msvc-14.0 ^
                                 ..

cmake --build . --config Release

ctest -C Release -V

ENDLOCAL

PAUSE
