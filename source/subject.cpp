#include "../headers/subject.h"
#include <iostream>

int Subject::f1(int a, int b) {
    return a + b;
}

double Subject::f2(double a, double b) {
    return a * b;
}

std::string Subject::f3(std::string a, std::string b) {
    return a + b;
}

float Subject::f4(float a, float b, float c) {
    return a + b + c;
}

int Subject::f5(int a, int b) {
    return a - b;
}

double Subject::f6(int a, double b, float c) {
    return a + b + c;
}

void Subject::f7(int a) {
    std::cout << "аргумент у void метода = " << a << std::endl;
}
