#pragma once
#include "expressions/addition.hpp"
#include "expressions/distributiveExpression.hpp"
#include "expressions/number.hpp"

#include <typeindex>
#include <vector>

struct Multiplication : public DistributiveBinaryExpression<true, Multiplication, Addition> {
  public:
    inline constexpr Multiplication() {
    }

    inline Multiplication(Expression* left, Expression* right)
        : DistributiveBinaryExpression(left, right) {
    }

    inline virtual constexpr std::optional<Number> getIdentity() const override {
        return 1;
    }

    inline virtual constexpr std::optional<Number> getAbsorbingElement() const override {
        return 0;
    }

    inline virtual Number getValue() const override {
        return left->getValue() * right->getValue();
    }

    inline virtual Expression* differentiate(const Variable* var) const override {
        return new Addition(new Multiplication(left->differentiate(var), right->copy()), new Multiplication(left->copy(), right->differentiate(var)));
    }

    virtual Expression* simplify() const override;

    inline virtual bool matches(const Expression* pattern) const override {
        return BinaryExpression<true>::matches(pattern);
    }

    inline virtual std::string to_string() const override {
        bool bracketsLeft = left->getType() == ExpressionTypes::Addition || left->isComplex();
        bool bracketsRight = right->getType() == ExpressionTypes::Addition || left->isComplex();

        const std::string& leftString = (bracketsLeft ? "(" + left->to_string() + ")" : left->to_string());
        const std::string& rightString = (bracketsRight ? "(" + right->to_string() + ")" : right->to_string());

        return leftString + " * " + rightString;
    }

    inline virtual ExpressionTypes getType() const override {
        return ExpressionTypes::Multiplication;
    }

    inline virtual Expression* copy() const override {
        return new Multiplication(left->copy(), right->copy());
    }

  protected:
    // static Expression* simplifyNumericalFactor(Expression* number, Expression* other);

    inline static Expression* fromFactors(const std::vector<const Expression*>& factors) {
        if (factors.size() == 0) {
            return new Number(1);
        }

        Expression* result = nullptr;

        for (const Expression* factor : factors) {
            if (factor->isNumeric()) {
                NumericType value = factor->getValue();
                if (value == 1) {
                    delete factor;
                    continue;
                }
            }

            result = result == nullptr ? factor->copy() : new Multiplication(result, factor->copy());
            delete factor;
        }

        return result;
    }

    static void mergeFactors(std::vector<const Expression*>& factors);
};

#undef left
#undef right
