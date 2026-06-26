@echo off
cd /d "%~dp0"

if not exist delivery_optimizer.exe (
    echo delivery_optimizer.exe was not found.
    echo Run build_and_run.bat first to build the project.
    pause
    exit /b 1
)

delivery_optimizer.exe

echo.
pause
