#include "defer.h"
#include <iostream>

int main() {
    int num = 0;
    {
        auto _ = base::defer([&] { num = 100; });
    }
    if (num == 100) {
        std::cout << "defer test success!" << std::endl;
    } else {
        std::cout << "defer test failed!" << std::endl;
    }
    return 0;
}
