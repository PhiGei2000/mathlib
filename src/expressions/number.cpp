#include "expressions/number.hpp"

Number Number::getValue() const {
    return Number(*this);
}

std::string Number::toString() const {
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