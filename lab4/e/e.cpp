extern "C" {
    #include "e.h"
}

float E() {
    float e = 1.0;
    float factorial = 1.0;
    
    for (int n = 1; n <= 10; n++) {
        factorial *= n;
        e += 1 / factorial;
    }
    
    return e;
}
