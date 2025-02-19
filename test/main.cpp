#include <expressions/expressions.hpp>

#include <iostream>

int main(int argC, char** argV) {
    auto x = Variable("x");
    auto y = Variable("y");

    auto two = Complex(2, 0);
    auto prod = mul(x, y);
    auto expr = pow(prod, two);

    auto expanded = expr.expand();

    std::cout << "expanding: " << expr.toString() << " = " << expanded->toString() << std::endl;

    std::cout << "Num of expressions: " << Expression::expressionCount << "\nDelete expanded" << std::endl;

    delete expanded;

    std::cout << "Num of expressions: " << Expression::expressionCount << std::endl;
}