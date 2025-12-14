#include <iostream>

#include "DynamicLoader.hpp"

#include <dlfcn.h>
using LibHandle = void*;

DynamicLoader::DynamicLoader() = default;

DynamicLoader::~DynamicLoader() {
    if (handle_) {
        dlclose(static_cast<LibHandle>(handle_));
        handle_ = nullptr;
    }
    E = nullptr;
    derivative_cos = nullptr;
}

bool DynamicLoader::Load(const std::string& path) {
    if (handle_) {
        dlclose(static_cast<LibHandle>(handle_));
        handle_ = nullptr;
        E = nullptr;
        derivative_cos = nullptr;
    }
    dlerror();
    LibHandle h = dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (!h) {
        const char* err = dlerror();
        lastErr = err ? err : "dlopen returned nullptr без сообщения";
        return false;
    }
    dlerror();
    void* sym1 = dlsym(h, "E");
    const char* err1 = dlerror();
    dlerror();
    void* sym2 = dlsym(h, "derivative_cos");
    const char* err2 = dlerror();
    if (err1 || err2 || !sym1 || !sym2) {
        std::string msg = "Не удалось найти символы: ";
        if (err1) msg += std::string("E: ") + err1 + "; ";
        else if (!sym1) msg += "E: не экспортируется; ";
        if (err2) msg += std::string("derivative_cos: ") + err2 + "; ";
        else if (!sym2) msg += "derivative_cos: не экспортируется; ";
        lastErr = msg;
        dlclose(h);
        return false;
    }
    handle_ = h;
    E = reinterpret_cast<float(*)()>(sym1);
    derivative_cos = reinterpret_cast<float(*)(float,float)>(sym2);
    lastErr.clear();
    return true;
}
