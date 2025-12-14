#include <iostream>

extern "C" {
    #include "cos.h"
}

float my_cos(float x) {
    const float PI = 3.14159265358979323846f;

    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;
    
    float ans = 0;
    float now = 1;
    int sign = 1;

    for (int n = 0; n < 10; n++) {
        ans += sign * now;
        now *= (x * x) / ((2*n + 1) * (2*n + 2));
        sign = -sign;
    }

    return ans;
}

float derivative_cos(float x, float deltaX = 0.0001f) {
    return (my_cos(x + deltaX) - my_cos(x)) / deltaX;
}
