#include <iostream>

#include "../e/e.h"
#include "../cos/cos.h"

int main() {
    float x, delta_x;
    std::cin >> x >> delta_x;
    std::cout << derivative_cos(x, delta_x) << std::endl;
    std::cout << E() << std::endl;
    return 0;
}