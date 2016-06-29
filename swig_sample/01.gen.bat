SETLOCAL

set SCRIPT=%~0
for /f "delims=\ tokens=*" %%z in ("%SCRIPT%") do (
  set SCRIPT_DRIVE=%%~dz
  set SCRIPT_PATH=%%~pz
  set SCRIPT_CURRENT_DIR=%%~dpz
)

mkdir java\swigtest1\generated
del java\swigtest1\generated\*.java
swig -c++ -java -package swigtest1.generated -outdir %SCRIPT_CURRENT_DIR%\java\swigtest1\generated c++\mydll.i

cd /d %SCRIPT_CURRENT_DIR%

ENDLOCAL
PAUSE
