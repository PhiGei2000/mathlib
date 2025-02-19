#pragma once
#include "expressions/addition.hpp"
#include "expressions/binaryExpression.hpp"
#include "expressions/number.hpp"

#include <typeindex>
#include <vector>

struct Multiplication : public BinaryExpression<true> {
  public:
    inline Multiplication(Expression* left, Expression* right)
        : BinaryExpression(left, right) {
    }

    inline virtual Number getValue() const override {
        return left->getValue() * right->getValue();
    }

    inline virtual std::string toString() const override {
        bool bracketsLeft = left->getType() == ExpressionTypes::Addition;
        bool bracketsRight = right->getType() == ExpressionTypes::Addition;

        const std::string& leftString = (bracketsLeft ? "(" + left->toString() + ")" : left->toString());
        const std::string& rightString = (bracketsRight ? "(" + right->toString() + ")" : right->toString());

        return leftString + " * " + rightString;
    }

    inline virtual ExpressionTypes getType() const override {
        return ExpressionTypes::Multiplication;
    }

    virtual Expression* expand() const override;

    inline virtual Expression* copy() const override {
        return new Multiplication(left->copy(), right->copy());
    }
};

template<ExpressionType T1, ExpressionType T2>
Multiplication mul(T1& first, T2& second) {
    return Multiplication(&first, &second);
}
