#pragma once
#include "expressions/binaryExpression.hpp"
#include "expressions/number.hpp"
#include "expressions/variable.hpp"

struct Addition : public BinaryExpression<true> {
  public:
    inline Addition(Expression* left, Expression* right)
        : BinaryExpression(left, right) {
    }

    inline virtual Number getValue() const override {
        return left->getValue() + right->getValue();
    }

    inline virtual Expression* differentiate(const Variable* var) const override {
        return new Addition(left->differentiate(var), right->differentiate(var));
    }

    inline virtual Expression* simplify() const override {
        Expression* simplifiedLeft = left->simplify();
        Expression* simplifiedRight = right->simplify();

        switch (simplifiedLeft->getType()) {
            case ExpressionTypes::Number:
                return simplifyNummericalSummand(simplifiedLeft, simplifiedRight);
            default: break;
        }

        switch (simplifiedRight->getType()) {
            case ExpressionTypes::Number:
                return simplifyNummericalSummand(simplifiedRight, simplifiedLeft);
            default: break;
        }

        std::vector<const Expression*> summands;
        getSummands(simplifiedLeft, summands);
        getSummands(simplifiedRight, summands);

        // TODO: identify like terms and merge summands


        return new Addition(simplifiedLeft, simplifiedRight);
    }

    inline virtual std::string to_string() const override {
        return left->to_string() + " + " + right->to_string();
    }

    inline virtual ExpressionTypes getType() const override {
        return ExpressionTypes::Addition;
    }

    inline virtual Expression* copy() const override {
        return new Addition(left->copy(), right->copy());
    }

  protected:
    inline static Expression* simplifyNummericalSummand(Expression* number, Expression* other) {
        Number value1 = number->getValue();
        delete number;

        if (value1 == 0) {
            return other;
        }

        if (other->getType() == ExpressionTypes::Number) {
            Number value2 = other->getValue();

            delete other;

            return new Number(value1 + value2);
        }

        return new Addition(new Number(value1), other);
    }
};

template<ExpressionType T1, ExpressionType T2>
inline Addition add(T1& first, T2& second) {
    return Addition(&first, &second);
}
