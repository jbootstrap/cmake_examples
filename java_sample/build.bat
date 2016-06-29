set SCRIPT=%~0
for /f "delims=\ tokens=*" %%z in ("%SCRIPT%") do (
  set SCRIPT_DRIVE=%%~dz
  set SCRIPT_PATH=%%~pz
  set SCRIPT_CURRENT_DIR=%%~dpz
)

SETLOCAL

cd /d %SCRIPT_CURRENT_DIR%
rmdir /s /q cmake_vc2013
mkdir cmake_vc2013
cd cmake_vc2013
cmake -G "Visual Studio 12 2013" -DCMAKE_CXX_FLAGS_RELEASE="/MT" -DCMAKE_CXX_FLAGS_DEBUG="/MTd" ^
                                 ..\src
cmake --build . --config Release
rem cmake --build . --config Debug

ctest -C Release -V
rem ctest -C Debug -V

:SUCCESS
echo ����ɏI�����܂����B
goto END

:ERR
echo �G���[���������܂����B
goto END

:END

ENDLOCAL

PAUSE