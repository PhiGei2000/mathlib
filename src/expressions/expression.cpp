#include "expressions/expression.hpp"

#include "expressions/addition.hpp"
#include "expressions/binaryExpression.hpp"
#include "expressions/expressions.hpp"
#include "expressions/multiplication.hpp"
#include "expressions/number.hpp"
#include "expressions/variable.hpp"

template<>
ExpressionTypes getExpressionType<Addition>() {
    return ExpressionTypes::Addition;
}

template<>
ExpressionTypes getExpressionType<Multiplication>() {
    return ExpressionTypes::Multiplication;
}

template<>
ExpressionTypes getExpressionType<Exponentiation>() {
    return ExpressionTypes::Exponentiation;
}

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
