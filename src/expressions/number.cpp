#include "expressions/number.hpp"

#include <cmath>

Number Number::getValue() const {
    return Number(*this);
}

std::string Number::to_string() const {
    return std::to_string(value);
}

ExpressionTypes Number::getType() const {
    return ExpressionTypes::Number;
}

bool Number::matches(const Expression* pattern) const {
    if (pattern->getType() != ExpressionTypes::Number)
        return false;

    return value == pattern->getValue();
}

Number Number::operator+(const Number& other) const {
    return Number(value + other.value);
}

Number Number::operator-() const {
    return Number(-value);
}

Number Number::operator-(const Number& other) const {
    return Number(value - other.value);
}

Number Number::operator*(const Number& other) const {
    return Number(value * other.value);
}

Number Number::operator/(const Number& other) const {
    return Number(value / other.value);
}

Number::operator double() const {
    return value;
}

Number Number::operator^(const Number& exponent) const {
    return Number(std::pow(value, exponent.value));
}