#pragma once
#include "binaryExpression.hpp"

#include "multiplication.hpp"
#include "number.hpp"

#include <math.h>

struct Exponentiation : public BinaryExpression<false> {
  public:
    inline Exponentiation(Expression* base, Expression* exp)
        : BinaryExpression(base, exp) {
    }

    inline virtual Number getValue() const override {
        return std::pow(left->getValue(), right->getValue());
    }

    inline virtual std::string toString() const override {
        bool leftBrackets = left->getType() > ExpressionTypes::Exponentiation;
        bool rightBrackets = right->getType() >= ExpressionTypes::Exponentiation;

        const std::string& leftString = leftBrackets ? "(" + left->toString() + ")" : left->toString();
        const std::string& rightString = rightBrackets ? "(" + right->toString() + ")" : right->toString();

        return leftString + " ^ " + rightString;
    }

    inline virtual ExpressionTypes getType() const override {
        return ExpressionTypes::Exponentiation;
    }

    inline virtual Expression* copy() const override {
        return new Exponentiation(left->copy(), right->copy());
    }

    virtual Expression* expand() const override;
};


template<ExpressionType TBase, ExpressionType TExp>
Exponentiation pow(TBase& base, TExp& exponent) {
    return Exponentiation(&base, &exponent);
}

