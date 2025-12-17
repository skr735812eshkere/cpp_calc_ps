# cpp_calc_ps
Первое домашнее задание по Науке Программирования

## Инструкция по запуску:
### Windows
сборка плагинов:
```
cmake -S . -B .build -G Ninja
cmake --build .build
cd .build
```
запуск:
```
./calc.exe
```

### MacOs
сборка плагинов:
```
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```
запуск:
```
./calc
```
## Поддерживает:
1. Арифметика: +, -, *, /
2. Функции (через плагины): sin, cos, tg, ctg
3. Динамическая загрузка плагинов из plugins/
4. Кроссплатформенное приложение

