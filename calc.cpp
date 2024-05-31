#include <iostream>
#include <cmath>
#include <cassert>

class Calculator {
public:
    float add(float a, float b) {
        return a + b;
    }

    float subtract(float a, float b) {
        return a - b;
    }

    float multiply(float a, float b) {
        return a * b;
    }

    float divide(float a, float b) {
        assert(b != 0); // Ensure division by zero doesn't occur
        return a / b;
    }
};

// Unit tests
int main() {
    Calculator calc;

    // Test addition
    assert(calc.add(5, 3) == 8);

    // Test subtraction
    assert(calc.subtract(10, 4) == 6);

    // Test multiplication
    assert(calc.multiply(2, 3) == 6);

    // Test division
    assert(calc.divide(10, 2) == 5);

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
