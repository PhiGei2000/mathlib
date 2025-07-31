#include <expressions/expressions.hpp>

#include <iostream>

int max_depth = 3;

Expression* createExpression(int depth = 0) {
    ExpressionTypes type = static_cast<ExpressionTypes>(rand() % (depth < max_depth ? 5 : 2) + 1);

    switch (type) {
        case ExpressionTypes::Number:
            return new Number(rand() / static_cast<double>(RAND_MAX));
        case ExpressionTypes::Variable:
            return new Variable(std::string("x"));
        case ExpressionTypes::Addition: {
            Expression* left = createExpression(depth + 1);
            Expression* right = createExpression(depth + 1);

            return add(left, right);
        }
        case ExpressionTypes::Multiplication: {
            Expression* left = createExpression(depth + 1);
            Expression* right = createExpression(depth + 1);

            return mul(left, right);
        }
        case ExpressionTypes::Exponentiation: {
            Expression* left = createExpression(depth + 1);
            Expression* right = createExpression(depth + 1);

            return pow(left, right);
        }
    }
}

int main(int argC, char** argV) {
    for (int i =0; i < 100; i++) {
        Expression* expr = createExpression();

        if (static_cast<int>(expr->getType()) <= 2) {
            delete expr;
            i--;
            continue;
        }

        auto simplified = expr->simplify();

        std::cout << expr->to_string() << " = " << simplified->to_string() << std::endl;

        delete simplified;
        delete expr;

    }
}
