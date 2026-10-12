@echo off
REM Run this from the project root (the folder that contains Source, Data, Executable).
REM The .cpp files are listed explicitly so old/scratch files in Source are never compiled by accident.
g++ -std=c++17 -Wall -Wextra Source\main.cpp Source\Utils.cpp Source\MenuItem.cpp Source\Customer.cpp Source\Order.cpp Source\Bill.cpp Source\Payment.cpp Source\PaymentTypes.cpp Source\FileManager.cpp Source\RestaurantSystem.cpp -o Executable\Project.exe
if %errorlevel% neq 0 (
    echo BUILD FAILED
    exit /b 1
)
echo BUILD OK - run it with:  Executable\Project.exe
