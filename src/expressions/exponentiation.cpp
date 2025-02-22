#include "expressions/exponentiation.hpp"

Expression* Exponentiation::expand() const {
    std::vector<const Expression*> leftFactors;

    Expression* left = this->left->expand();
    getFactors(left, leftFactors);

    // (a * b) ^ c = a ^ c * b ^ c
    Expression* result = new Exponentiation(leftFactors[leftFactors.size() - 1]->copy(), right->copy());
    for (int l = leftFactors.size() - 2; l >= 0; l--) {
        result = new Multiplication(result, new Exponentiation(leftFactors[l]->copy(), right->copy()));
    }

    delete left;

    return result;
}

Expression* Exponentiation::simplifyNumericalBase(Expression* base, Expression* exponent) {
    Number baseValue = base->getValue();
    delete base;

    switch (exponent->getType()) {
        case ExpressionTypes::Number: {
            Number exponentValue = exponent->getValue();

            delete exponent;

            return new Number(baseValue ^ exponentValue);
        }
        default:
            break;
    }

    return new Exponentiation(new Number(baseValue), exponent);
}

Expression* Exponentiation::simplifyNumericalExponent(Expression* base, Expression* exponent) {
    Number exponentValue = exponent->getValue();
    delete exponent;

    switch (base->getType()) {
        case ExpressionTypes::Number: {
            Number baseValue = base->getValue();

            delete base;
            return new Number(baseValue ^ exponentValue);
        }
        default:
            break;
    }

    return new Exponentiation(base, new Number(exponentValue));
}