@echo off
cd /d "%~dp0"

echo Building Last-Mile Delivery Route Optimizer...
g++ -std=c++20 src/main.cpp src/Graph.cpp src/CsvReader.cpp src/RoutePlanner.cpp src/VehicleAssigner.cpp src/Metrics.cpp -Iinclude -o delivery_optimizer.exe

if errorlevel 1 (
    echo.
    echo Build failed. Please check that MinGW g++ is installed and added to PATH.
    pause
    exit /b 1
)

echo.
echo Running project...
echo.
delivery_optimizer.exe

echo.
pause
