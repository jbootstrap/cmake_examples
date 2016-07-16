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
cmake -G "Visual Studio 14 2015" -DCMAKE_CXX_FLAGS_RELEASE="/MD /clr /EHa" -DCMAKE_CXX_FLAGS_DEBUG="/MDd /clr /EHa" ^
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
