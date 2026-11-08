@echo off
setlocal

if "%~1"=="" (
    set "OUTPUT_NAME=vengeance.exe"
) else (
    set "OUTPUT_NAME=%~1"
)

echo Compiling program into %OUTPUT_NAME%...

cl -O2 /GS- /EHsc /I. /D WIN32_LEAN_AND_MEAN /Fe:%OUTPUT_NAME% *.cpp
if %errorlevel% equ 0 (
    echo Compilation successful!
) else (
    echo Compilation failed.
)

endlocal
pause