#include <iostream>
#include <cassert>
#include "../headers/subject.h"
#include "../headers/wrapper.h"
#include "../headers/engine.h"

void test1() {
    Subject s;
    auto w = std::make_unique<Wrapper2<Subject, int, int, int>>(
        &s, &Subject::f1,
        std::vector<std::pair<std::string, std::any>>{{"arg1", 0}, {"arg2", 0}}
    );
    Engine e;
    e.registerCommand("f1", std::move(w));
    int r = e.execute<int>("f1", {{"arg1", 11}, {"arg2", 12}});
    std::cout << "test 1: " << r << " (ожидается 23)" << std::endl;
    assert(r == 23);
}

void test2() {
    Subject s;
    auto w = std::make_unique<Wrapper2<Subject, double, double, double>>(
        &s, &Subject::f2,
        std::vector<std::pair<std::string, std::any>>{{"arg1", 2.0}, {"arg2", 1.5}}
    );
    Engine e;
    e.registerCommand("f2", std::move(w));
    double r = e.execute<double>("f2", {{"arg1", 3.0}});
    std::cout << "test 2: " << r << " (ожидается 4.5)" << std::endl;
    assert(std::abs(r - 4.5) < 0.001);
}

void test3() {
    Subject s;
    auto w = std::make_unique<Wrapper2<Subject, std::string, std::string, std::string>>(
        &s, &Subject::f3,
        std::vector<std::pair<std::string, std::any>>{{"arg1", std::string("My name is ")}, {"arg2", std::string("Anastasiya")}}
    );
    Engine e;
    e.registerCommand("f3", std::move(w));
    std::string r = e.execute<std::string>("f3", {{"arg2", std::string("Nazarova Anastasiya")}});
    std::cout << "test 3: " << r << " (ожидается My name is Nazarova Anastasiya)" << std::endl;
    assert(r == "My name is Nazarova Anastasiya");
}

void test4() {
    Subject s;
    
    auto w = std::make_unique<Wrapper3<Subject, float, float, float, float>>(
        &s, &Subject::f4,
        std::vector<std::pair<std::string, std::any>>{{"arg1", 1.0f}, {"arg2", 2.0f}, {"arg3", 3.0f}}
    );
    
    Engine e;
    e.registerCommand("f4", std::move(w));
    
    float r = e.execute<float>("f4", {{"arg1", 12.0f}, {"arg3", 34.0f}});
    std::cout << "test 4: " << r << " (ожидается 48)" << std::endl;
    assert(std::abs(r - 48.0f) < 0.001);
}

void test5() {
    auto w = std::make_unique<StaticWrapper2<int, int, int>>(
        &Subject::f5,
        std::vector<std::pair<std::string, std::any>>{{"arg1", 0}, {"arg2", 0}}
    );
    
    Engine e;
    e.registerCommand("f5", std::move(w));
    
    int r = e.execute<int>("f5", {{"arg1", 10}, {"arg2", 3}});
    std::cout << "test 5: " << r << " (ожидается 7)" << std::endl;
    assert(r == 7);
}

void test6() {
    Subject s;
    
    auto w = std::make_unique<Wrapper3<Subject, double, int, double, float>>(
        &s, &Subject::f6,
        std::vector<std::pair<std::string, std::any>>{{"arg1", 0}, {"arg2", 0.0}, {"arg3", 0.0f}}
    );
    
    Engine e;
    e.registerCommand("f6", std::move(w));
    
    double r = e.execute<double>("f6", {{"arg1", 10}, {"arg2", 20.5}, {"arg3", 30.2f}});
    std::cout << "test 6: " << r << " (ожидается 60.7)" << std::endl;
    assert(std::abs(r - 60.7) < 0.001);
}

void test7() {
    Subject s;
    
    auto w = std::make_unique<Wrapper1<Subject, void, int>>(
        &s, &Subject::f7,
        std::vector<std::pair<std::string, std::any>>{{"arg1", 0}}
    );
    
    Engine e;
    e.registerCommand("f7", std::move(w));
    
    e.execute<void>("f7", {{"arg1", 23}});
    std::cout << "test 7: void метод вызван" << std::endl;
}

void test8() {
    Subject s;
    
    auto w = std::make_unique<Wrapper2<Subject, int, int, int>>(
        &s, &Subject::f1,
        std::vector<std::pair<std::string, std::any>>{{"arg1", 0}, {"arg2", 0}}
    );
    
    Engine e;
    e.registerCommand("cmd", std::move(w));
    
    try {
        int r = e.execute<int>("cmd", {{"arg1", 4}});
        std::cout << "test 8: " << r << " (ожидается 4)" << std::endl;
        assert(r == 4);
    } catch (...) {
        std::cout << "test 8 провален" << std::endl;
        assert(false);
    }
}

void test9() {
    Engine e;
    
    try {
        e.execute<int>("nonexistent", {});
        std::cout << "test 9 должен был вызвать исключение" << std::endl;
        assert(false);
    } catch (const std::invalid_argument&) {
        std::cout << "test 9: корректное исключение для несуществующей команды" << std::endl;
    }
}

void test10() {
    Subject s;
    
    auto w = std::make_unique<Wrapper2<Subject, int, int, int>>(
        &s, &Subject::f1,
        std::vector<std::pair<std::string, std::any>>{{"arg1", 0}, {"arg2", 0}}
    );
    
    Engine e;
    e.registerCommand("cmd", std::move(w));
    
    try {
        e.execute<int>("cmd", {{"arg1", 4}, {"arg2", std::string("wrong")}});
        std::cout << "test 10 должен был вызвать исключение для неверного типа" << std::endl;
        assert(false);
    } catch (const std::invalid_argument&) {
        std::cout << "test 10: корректное исключение для неверного типа" << std::endl;
    }
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();
    return 0;
}
