#pragma once
#include "binaryExpression.hpp"

#include "multiplication.hpp"
#include "number.hpp"

#ifdef WIN32
#include <cmath>
#else
#include <math.h>
#endif

struct Exponentiation : public BinaryExpression<false> {
  public:
    inline Exponentiation(Expression* base, Expression* exp)
        : BinaryExpression(base, exp) {
    }

    inline virtual Number getValue() const override {
        return std::pow(left->getValue(), right->getValue());
    }

    inline virtual Expression* differentiate(const Variable* var) const override {
        // d/dx f^g = f ^ g * (g' * ln f + g / f * f')

        return new Multiplication(copy(),
                                  new Addition(
                                      new Multiplication(right->differentiate(var), /*ln*/ left->copy()),
                                      new Multiplication(right->copy(),
                                                         new Multiplication(new Exponentiation(left->copy(), new Number(-1)), left->differentiate(var)))));
    }

    inline virtual Expression* simplify() const override {
        auto [simplifiedLeft, simplifiedRight] = simplifyChildren();

        switch (simplifiedLeft->getType()) {
            case ExpressionTypes::Number:
                return simplifyNumericalBase(simplifiedLeft, simplifiedRight);
            case ExpressionTypes::Exponentiation: {
                Exponentiation* base = reinterpret_cast<Exponentiation*>(simplifiedLeft);
                base->right = new Multiplication(base->right, simplifiedRight);

                Expression* simplified = base->simplify();
                delete base;

                return simplified;
            } break;
            default:
                break;
        }

        switch (simplifiedRight->getType()) {
            case ExpressionTypes::Number:
                return simplifyNumericalExponent(simplifiedLeft, simplifiedRight);
            default:
                break;
        }

        return new Exponentiation(simplifiedLeft, simplifiedRight);
    }

    inline virtual std::string to_string() const override {
        bool leftBrackets = left->getType() > ExpressionTypes::Exponentiation || left->isComplex();
        bool rightBrackets = right->getType() >= ExpressionTypes::Exponentiation || right->isComplex();

        const std::string& leftString = leftBrackets ? "(" + left->to_string() + ")" : left->to_string();
        const std::string& rightString = rightBrackets ? "(" + right->to_string() + ")" : right->to_string();

        return leftString + " ^ " + rightString;
    }

    inline virtual ExpressionTypes getType() const override {
        return ExpressionTypes::Exponentiation;
    }

    inline virtual Expression* copy() const override {
        return new Exponentiation(left->copy(), right->copy());
    }

    virtual Expression* expand() const override;

  protected:
    static Expression* simplifyNumericalBase(Expression* base, Expression* exponent);
    static Expression* simplifyNumericalExponent(Expression* base, Expression* exponent);
};
