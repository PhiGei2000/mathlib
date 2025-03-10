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

    inline virtual Expression* differentiate(const Variable* var) const override {
        return new Addition(new Multiplication(left->differentiate(var), right->copy()), new Multiplication(left->copy(), right->differentiate(var)));
    }

    virtual Expression* simplify() const override;

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

    virtual Expression* expand() const override;

    inline virtual Expression* copy() const override {
        return new Multiplication(left->copy(), right->copy());
    }

  protected:
    static Expression* simplifyNumericalFactor(Expression* number, Expression* other);

    inline static Expression* fromFactors(const std::vector<const Expression*>& factors) {
        switch (factors.size()) {
            case 0:
                return new Number(1);
            default: {
                Expression* result = factors.front()->copy();

                for (int i = 1; i < factors.size(); i++) {
                    result = new Multiplication(result, factors[i]->copy());
                }

                return result;
            }
        }
    }

    Expression* simplifyExpandable() const;
    static void mergeFactors(Number& numericalFactor, std::vector<const Expression*>& factors, std::vector<const Expression*>& mergedFactors);
};

template<ExpressionType T1, ExpressionType T2>
Multiplication mul(T1& first, T2& second) {
    return Multiplication(&first, &second);
}

template<ExpressionType T1, ExpressionType T2>
Multiplication* mul(T1* first, T2* second) {
    return new Multiplication(
        first->parent != nullptr ? first->copy() : first,
        second->parent != nullptr ? second->copy() : second);
}
