@echo off
setlocal

if "%~1"=="" (
    set "OUTPUT_NAME=vengeance.exe"
) else (
    set "OUTPUT_NAME=%~1"
)

if not exist "bin" mkdir "bin"

echo Compiling program into bin\%OUTPUT_NAME%...

if exist "bin\%OUTPUT_NAME%" del /f /q "bin\%OUTPUT_NAME%"

cl -O2 /GS- /EHsc /Isrc /D WIN32_LEAN_AND_MEAN /Fe:bin\%OUTPUT_NAME% src\*.cpp

if %errorlevel% equ 0 (
    echo Compilation successful!
) else (
    echo Compilation failed.
)

endlocal
pause