#include "expressions/expression.hpp"

#include "expressions/addition.hpp"
#include "expressions/binaryExpression.hpp"
#include "expressions/expressions.hpp"
#include "expressions/multiplication.hpp"
#include "expressions/number.hpp"
#include "expressions/variable.hpp"

int Expression::expressionCount = 0;
std::vector<const Expression*> Expression::expressions = {};

std::set<Variable> Expression::getVariables() const {
    return {};
}

bool Expression::areLike(const Expression* first, const Expression* second, Number& factor) {
    Multiplication* quotient = div(first, second);
    Expression* simplifiedQuotient = quotient->simplify();

    delete quotient;
    bool like = simplifiedQuotient->isNumeric();
    factor = like ? simplifiedQuotient->getValue() : Number(0);

    delete simplifiedQuotient;

    return like;
}

Expression* Expression::differentiate(const Variable* var) const {
    return new Number(0);
}

void Expression::operator delete(void* ptr) {
    Expression* expr = static_cast<Expression*>(ptr);
    if (expr->isBinary()) {
        BinaryExpressionBase* binExpr = static_cast<BinaryExpressionBase*>(ptr);
        Expression::operator delete(binExpr->left);
        Expression::operator delete(binExpr->right);
    }

    expressionCount -= 1;
    expressions.erase(std::ranges::find(expressions.begin(), expressions.end(), expr));

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