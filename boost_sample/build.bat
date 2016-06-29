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
                                 -DBOOST_LIBRARYDIR=C:\usr\local\boost_1_61_0\lib32-msvc-12.0 ^
                                 ..

cmake --build . --config Release
rem cmake --build . --config Debug

:SUCCESS
echo 正常に終了しました。
goto END

:ERR
echo エラーが発生しました。
goto END

:END

ENDLOCAL

PAUSE
