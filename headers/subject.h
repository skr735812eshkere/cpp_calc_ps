#pragma once
#include <string>

class Subject {
public:
    int f1(int a, int b);
    double f2(double a, double b);
    std::string f3(std::string a, std::string b);
    float f4(float a, float b, float c);
    static int f5(int a, int b);
    double f6(int a, double b, float c);
    void f7(int a);
};
