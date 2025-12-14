#include "DynamicLoader.hpp"
#include <iostream>

int main() {
    DynamicLoader loader;
    const char* names[] = {"contract2.so", "./contract2.so", "build/contract2.so", nullptr};
    if (!loader.Load("./contract2.so")) {
        std::cerr << "Ошибка загрузки: " << loader.Error() << std::endl;
        return 1;
    }
    std::cout << "e = " << loader.E()   << "\n";
    std::cout << "derivative_cos(1.0f, 0.001f) = " << loader.derivative_cos(1.0f, 0.001f) << "\n";
    return 0;
}
