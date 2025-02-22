#include <expressions/expressions.hpp>

#include <iostream>

int main(int argC, char** argV) {
    auto x = Variable("x");
    auto y = Variable("y");

    auto two = Number(3);
    auto five = Number(5);
    auto prod = mul(five, y);
    auto expr = pow(prod, two);
    auto sum = add(x, prod);

    auto result = expr.differentiate(&y);
    auto simplifiedResult = result->simplify();

    std::cout << "d/dy " << expr.toString() << " = " << simplifiedResult->toString() << std::endl;

    std::cout << "Num of expressions: " << Expression::expressionCount << "\nDelete result" << std::endl;

    delete result;
    delete simplifiedResult;

    std::cout << "Num of expressions: " << Expression::expressionCount;
    for (const Expression* expr : Expression::expressions) {
        std::cout << "\n\t" << expr->toString();
    }

    std::cout << std::endl;
}