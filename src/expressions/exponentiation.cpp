#include "expressions/exponentiation.hpp"

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

    if (exponentValue == 0) {
        delete base;
        return new Number(1);
    }

    switch (base->getType()) {
        case ExpressionTypes::Number: {
            Number baseValue = base->getValue();

            delete base;
            return new Number(baseValue ^ exponentValue);
        }
        break;
        default:
            break;
    }

    return new Exponentiation(base, new Number(exponentValue));
}