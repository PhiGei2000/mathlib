#include "expressions/expression.hpp"

#include "expressions/binaryExpression.hpp"
#include "expressions/addition.hpp"
#include "expressions/multiplication.hpp"

int Expression::expressionCount = 0;

void Expression::operator delete(void* ptr) {
    Expression* expr = static_cast<Expression*>(ptr);
    if (expr->isBinary()) {
        BinaryExpressionBase* binExpr = static_cast<BinaryExpressionBase*>(ptr);
        Expression::operator delete(binExpr->left);
        Expression::operator delete(binExpr->right);
    }

    expressionCount -= 1;

    return ::operator delete(ptr);
}


void Expression::getSummands(const Expression* expr, std::vector<const Expression*>& summands) {
    if (expr->getType() == ExpressionTypes::Addition) {
        const Addition* sum = static_cast<const Addition*>(expr);

        getSummands(sum->left, summands);
        getSummands(sum->right, summands);
    }
    else {
        summands.push_back(expr);
    }
}

void Expression::getFactors(const Expression* expr, std::vector<const Expression*>& factors) {
    if (expr->getType() == ExpressionTypes::Multiplication) {
        const Multiplication* product = static_cast<const Multiplication*>(expr);

        getFactors(product->left, factors);
        getFactors(product->right, factors);
    }
    else {
        factors.push_back(expr);
    }
}