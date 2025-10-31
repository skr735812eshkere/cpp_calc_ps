@echo off
setlocal enabledelayedexpansion

if not exist build mkdir build
if not exist plugins mkdir plugins

g++ -std=c++17 -I src src\*.cpp -o build\calc.exe -lstdc++fs
if errorlevel 1 (
    echo ошибка при сборке calc.exe
    pause
    exit /b 1
)

echo building plugins...
g++ -std=c++17 -shared -fPIC -I src -o plugins\funcsin.dll plugins_src\sin_plugin.cpp
g++ -std=c++17 -shared -fPIC -I src -o plugins\funcdeg.dll plugins_src\deg_plugin.cpp
g++ -std=c++17 -shared -fPIC -I src -o plugins\funccos.dll plugins_src\cos_plugin.cpp
g++ -std=c++17 -shared -fPIC -I src -o plugins\functan.dll plugins_src\tan_plugin.cpp
g++ -std=c++17 -shared -fPIC -I src -o plugins\funccot.dll plugins_src\cot_plugin.cpp


echo.
echo запуск программы:
echo build\calc.exe
echo.
pause
