#pragma once
#include "expressions/binaryExpression.hpp"
#include "expressions/number.hpp"

struct Addition : public BinaryExpression<true> {
  public:
    inline Addition(Expression* left, Expression* right)
        : BinaryExpression(left, right) {
    }

    inline virtual Number getValue() const override {
        return left->getValue() + right->getValue();
    }

    inline virtual std::string toString() const override {
        return left->toString() + " + " + right->toString();
    }

    inline virtual ExpressionTypes getType() const override {
        return ExpressionTypes::Addition;
    }

    inline virtual Expression* copy() const override {
        return new Addition(left->copy(), right->copy());
    }
};

template<ExpressionType T1, ExpressionType T2>
inline Addition add(T1& first, T2& second) {
  return Addition(&first, &second);
}
