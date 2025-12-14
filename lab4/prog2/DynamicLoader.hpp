#pragma once

#include <string>

#include "e.h"
#include "../cos/cos.h"

class DynamicLoader {
public:
    DynamicLoader();
    ~DynamicLoader();

    bool Load(const std::string& path);
    std::string Error() const { return lastErr; }

    float (*E)() = nullptr;
    float (*derivative_cos)(float, float) = nullptr;

private:
    void* handle_ = nullptr;
    std::string lastErr;
};