#include <expressions/expressions.hpp>

#include <iostream>

int main(int argC, char** argV) {
    auto x = Variable("x");
    auto y = Variable("y");

    auto two = Complex(2, 0);
    auto five = Complex(5, 0);
    auto prod = mul(five, y);
    auto expr = pow(prod, two);
    auto sum = add(x, prod);

    auto result = expr.differentiate(&x);

    std::cout << "expanding: " << expr.toString() << " = " << result->toString() << std::endl;

    std::cout << "Num of expressions: " << Expression::expressionCount << "\nDelete result" << std::endl;

    delete result;

    std::cout << "Num of expressions: " << Expression::expressionCount << std::endl;
}