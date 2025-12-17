#pragma once
#include "wrapper.h"
#include <map>
#include <memory>
#include <string>

class Engine {
public:
    Engine() = default;
    
    void registerCommand(const std::string& n, std::unique_ptr<BaseWrapper> w) {
        auto it = c.find(n);
        if (it != c.end()) {
            throw std::runtime_error("команда уже существует: " + n);
        }
        c[n] = std::move(w);
    }
    
    template<typename R>
    R execute(const std::string& n, const std::map<std::string, std::any>& args = {}) {
        auto it = c.find(n);
        if (it == c.end()) {
            throw std::invalid_argument("команда не найдена: " + n);
        }
        
        std::any r = it->second->call(args);
        
        if constexpr (std::is_void_v<R>) {
            return;
        } else {
            try {
                return std::any_cast<R>(r);
            } catch (const std::bad_any_cast&) {
                throw std::runtime_error("неправильный возвращаемый тип для команды: " + n);
            }
        }
    }
    
    bool hasCommand(const std::string& n) const {
        return c.find(n) != c.end();
    }
    
    void removeCommand(const std::string& n) {
        c.erase(n);
    }
    
    void clear() {
        c.clear();
    }

private:
    std::map<std::string, std::unique_ptr<BaseWrapper>> c;
};
