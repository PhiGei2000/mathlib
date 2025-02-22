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

    inline virtual std::string toString() const override {
        bool bracketsLeft = left->getType() == ExpressionTypes::Addition || left->isComplex();
        bool bracketsRight = right->getType() == ExpressionTypes::Addition || left->isComplex();

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

  protected:
    inline static Expression* simplifyNumericalFactor(Expression* number, Expression* other) {
        Number value = number->getValue();
        delete number;

        if (value == 0) {
            delete other;
            return new Number(0);
        }
        else if (value == 1) {
            return other;
        }
        else
            switch (other->getType()) {
                case ExpressionTypes::Number: {
                    Number product = value * other->getValue();
                    delete other;

                    return new Number(product);
                }
                case ExpressionTypes::Multiplication: {
                    std::vector<const Expression*> factors;
                    getFactors(other, factors);

                    Number result = value;
                    auto it = factors.begin();

                    while (it != factors.end()) {
                        if ((*it)->getType() == ExpressionTypes::Number) {
                            result *= (*it)->getValue();

                            it = factors.erase(it);
                        }
                        else {
                            it++;
                        }
                    }

                    Expression* nonNumericalFactors = fromFactors(factors);
                    delete other;

                    return new Multiplication(new Number(result), nonNumericalFactors);
                }
                default: break;
            }

        return new Multiplication(new Number(value), other);
    }

    inline static Expression* fromFactors(const std::vector<const Expression*>& factors) {
        switch (factors.size()) {
            case 0:
                return new Number(1);
            default: {
                Expression* result = factors.front()->copy();

                if (factors.front()->parent == nullptr) {
                    delete factors.front();
                }

                for (int i = 1; i < factors.size(); i++) {
                    result = new Multiplication(result, factors[i]->copy());
                    if (factors[i]->parent == nullptr) {
                        delete factors[i];
                    }
                }

                return result;
            }
        }
    }
};

template<ExpressionType T1, ExpressionType T2>
Multiplication mul(T1& first, T2& second) {
    return Multiplication(&first, &second);
}
