#!/bin/bash
set -e
mkdir -p build
mkdir -p plugins

# main
clang++ -std=c++17 -I src src/*.cpp -o build/calc

# plugins 
clang++ -std=c++17 -shared -fPIC -I src -o plugins/libsin.dylib plugins_src/sin_plugin.cpp
clang++ -std=c++17 -shared -fPIC -I src -o plugins/libcos.dylib plugins_src/cos_plugin.cpp
clang++ -std=c++17 -shared -fPIC -I src -o plugins/libtan.dylib plugins_src/tan_plugin.cpp
clang++ -std=c++17 -shared -fPIC -I src -o plugins/libcot.dylib plugins_src/cot_plugin.cpp
clang++ -std=c++17 -shared -fPIC -I src -o plugins/libdeg.dylib plugins_src/deg_plugin.cpp


echo "запуск программы.. ./build/calc"
