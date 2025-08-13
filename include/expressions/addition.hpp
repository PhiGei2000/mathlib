#pragma once
#include "expressions/binaryExpression.hpp"
#include "expressions/number.hpp"
#include "expressions/variable.hpp"

struct Addition : public BinaryExpression<true> {
  public:
    inline constexpr Addition() : BinaryExpression<true>() {
    }

    inline Addition(Expression* left, Expression* right)
        : BinaryExpression(left, right) {
    }

    inline virtual constexpr std::optional<Number> getIdentity() const override {
        return 0;
    }

    inline virtual Number getValue() const override {
        return left->getValue() + right->getValue();
    }

    inline virtual Expression* differentiate(const Variable* var) const override {
        return new Addition(left->differentiate(var), right->differentiate(var));
    }

    virtual Expression* simplify() const override;

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

    static void mergeSummands(std::vector<const Expression*>& summands);

    static Expression* fromSummands(const std::vector<const Expression*>& summands) {
        if (summands.size() == 0) {
            return new Number(0);
        }

        Expression* result = nullptr;
        for (const Expression* summand : summands) {
            if (summand->isNumeric()) {
                NumericType value = summand->getValue();
                if (value == 0) {
                    delete summand;
                    continue;
                }
            }

            result = result == nullptr ? summand->copy() : new Addition(result, summand->copy());
            delete summand;
        }

        return result;
    }
};
