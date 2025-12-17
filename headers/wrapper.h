#pragma once
#include <any>
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <type_traits>
#include <iostream>

class BaseWrapper {
public:
    virtual ~BaseWrapper() = default;
    virtual std::any call(const std::map<std::string, std::any>& args) = 0;
    virtual bool isVoid() const = 0;
};

template<typename T, typename R, typename A1>
class Wrapper1 : public BaseWrapper {
public:
    Wrapper1(T* obj, R(T::*func)(A1), const std::vector<std::pair<std::string, std::any>>& def): o(obj), f(func) {
        for (const auto& p : def) {
            d[p.first] = p.second;
        }
    }
    
    std::any call(const std::map<std::string, std::any>& args) override {
        A1 a1 = getArg<A1>("arg1", args);
        
        if constexpr (std::is_void_v<R>) {
            (o->*f)(a1);
            return std::any();
        } else {
            R r = (o->*f)(a1);
            return r;
        }
    }
    
    bool isVoid() const override {
        return std::is_void_v<R>;
    }

private:
    T* o;
    R(T::*f)(A1);
    std::map<std::string, std::any> d;
    
    template<typename ArgT>
    ArgT getArg(const std::string& n, const std::map<std::string, std::any>& args) {
        auto it = args.find(n);
        if (it != args.end()) {
            try {
                return std::any_cast<ArgT>(it->second);
            } catch (const std::bad_any_cast&) {
                throw std::invalid_argument("неверный тип для аргумента " + n);
            }
        }
        it = d.find(n);
        if (it != d.end()) {
            try {
                return std::any_cast<ArgT>(it->second);
            } catch (const std::bad_any_cast&) {
                throw std::invalid_argument("неверный тип для default аргумента " + n);
            }
        }
        throw std::invalid_argument("отсутствует аргумент " + n);
    }
};

template<typename T, typename R, typename A1, typename A2>
class Wrapper2 : public BaseWrapper {
public:
    Wrapper2(T* obj, R(T::*func)(A1, A2), const std::vector<std::pair<std::string, std::any>>& def): o(obj), f(func) {
        for (const auto& p : def) {
            d[p.first] = p.second;
        }
    }
    
    std::any call(const std::map<std::string, std::any>& args) override {
        A1 a1 = getArg<A1>("arg1", args);
        A2 a2 = getArg<A2>("arg2", args);
        
        if constexpr (std::is_void_v<R>) {
            (o->*f)(a1, a2);
            return std::any();
        } else {
            R r = (o->*f)(a1, a2);
            return r;
        }
    }
    
    bool isVoid() const override {
        return std::is_void_v<R>;
    }

private:
    T* o;
    R(T::*f)(A1, A2);
    std::map<std::string, std::any> d;
    
    template<typename ArgT>
    ArgT getArg(const std::string& n, const std::map<std::string, std::any>& args) {
        auto it = args.find(n);
        if (it != args.end()) {
            try {
                return std::any_cast<ArgT>(it->second);
            } catch (const std::bad_any_cast&) {
                throw std::invalid_argument("неверный тип для аргумента " + n);
            }
        }
        it = d.find(n);
        if (it != d.end()) {
            try {
                return std::any_cast<ArgT>(it->second);
            } catch (const std::bad_any_cast&) {
                throw std::invalid_argument("неверный тип для default аргумента " + n);
            }
        }
        throw std::invalid_argument("отсутствует аргумент " + n);
    }
};

template<typename T, typename R, typename A1, typename A2, typename A3>
class Wrapper3 : public BaseWrapper {
public:
    Wrapper3(T* obj, R(T::*func)(A1, A2, A3), const std::vector<std::pair<std::string, std::any>>& def): o(obj), f(func) {
        for (const auto& p : def) {
            d[p.first] = p.second;
        }
    }
    
    std::any call(const std::map<std::string, std::any>& args) override {
        A1 a1 = getArg<A1>("arg1", args);
        A2 a2 = getArg<A2>("arg2", args);
        A3 a3 = getArg<A3>("arg3", args);
        
        if constexpr (std::is_void_v<R>) {
            (o->*f)(a1, a2, a3);
            return std::any();
        } else {
            R r = (o->*f)(a1, a2, a3);
            return r;
        }
    }
    
    bool isVoid() const override {
        return std::is_void_v<R>;
    }

private:
    T* o;
    R(T::*f)(A1, A2, A3);
    std::map<std::string, std::any> d;
    
    template<typename ArgT>
    ArgT getArg(const std::string& n, const std::map<std::string, std::any>& args) {
        auto it = args.find(n);
        if (it != args.end()) {
            try {
                return std::any_cast<ArgT>(it->second);
            } catch (const std::bad_any_cast&) {
                throw std::invalid_argument("неверный тип для аргумента " + n);
            }
        }
        it = d.find(n);
        if (it != d.end()) {
            try {
                return std::any_cast<ArgT>(it->second);
            } catch (const std::bad_any_cast&) {
                throw std::invalid_argument("неверный тип для default аргумента " + n);
            }
        }
        throw std::invalid_argument("отсутствует аргумент " + n);
    }
};

template<typename R, typename A1, typename A2>
class StaticWrapper2 : public BaseWrapper {
public:
    StaticWrapper2(R(*func)(A1, A2), const std::vector<std::pair<std::string, std::any>>& def): f(func) {
        for (const auto& p : def) {
            d[p.first] = p.second;
        }
    }
    
    std::any call(const std::map<std::string, std::any>& args) override {
        A1 a1 = getArg<A1>("arg1", args);
        A2 a2 = getArg<A2>("arg2", args);
        
        if constexpr (std::is_void_v<R>) {
            f(a1, a2);
            return std::any();
        } else {
            R r = f(a1, a2);
            return r;
        }
    }
    
    bool isVoid() const override {
        return std::is_void_v<R>;
    }

private:
    R(*f)(A1, A2);
    std::map<std::string, std::any> d;
    
    template<typename ArgT>
    ArgT getArg(const std::string& n, const std::map<std::string, std::any>& args) {
        auto it = args.find(n);
        if (it != args.end()) {
            try {
                return std::any_cast<ArgT>(it->second);
            } catch (const std::bad_any_cast&) {
                throw std::invalid_argument("неверный тип для аргумента " + n);
            }
        }
        it = d.find(n);
        if (it != d.end()) {
            try {
                return std::any_cast<ArgT>(it->second);
            } catch (const std::bad_any_cast&) {
                throw std::invalid_argument("неверный тип для default аргумента " + n);
            }
        }
        throw std::invalid_argument("отсутствует аргумент " + n);
    }
};
