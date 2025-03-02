#include "expressions/complex.hpp"

#include <cmath>

#include <numbers>

Complex::Complex(const std::tuple<NumericType, NumericType>& polarForm)
    : Number(std::get<0>(polarForm) * cos(std::get<1>(polarForm))), imaginary(std::get<0>(polarForm) * sin(std::get<1>(polarForm))) {
}

Number Complex::getValue() const {
    return Complex(*this);
}

Complex Complex::conjugate() const {
    return Complex(value, -imaginary);
}

std::tuple<NumericType, NumericType> Complex::polarForm() const {
    NumericType abs = sqrt(value * value + imaginary * imaginary);
    NumericType arg = value == 0 ? (imaginary < 0 ? -1 : 1) * std::numbers::pi / 2 : atan2(imaginary, value);

    return std::tuple<NumericType, NumericType>(abs, arg);
}

std::string Complex::to_string() const {
    if (imaginary == 0) {
        return Number::to_string();
    }

    if (value == 0) {
        return std::to_string(imaginary) + "i";
    }

    if (parent != nullptr && parent->getType() == ExpressionTypes::Multiplication) {
        return "(" + std::to_string(value) + (imaginary < 0 ? (" - " + std::to_string(-imaginary)) : " + " + std::to_string(imaginary)) + "i)";
    }

    return std::to_string(value) + (imaginary < 0 ? (" - " + std::to_string(-imaginary)) : " + " + std::to_string(imaginary)) + "i";
}

Complex Complex::operator+(const Complex& other) const {
    return Complex(value + other.value, imaginary + other.imaginary);
}

Complex Complex::operator-() const {
    return Complex(-value, -imaginary);
}

Complex Complex::operator-(const Complex& other) const {
    return Complex(value - other.value, imaginary - other.imaginary);
}

Complex Complex::operator*(const Complex& other) const {
    return Complex(value * other.value - imaginary * other.imaginary, value * other.imaginary + imaginary * other.value);
}

Complex Complex::operator/(const NumericType& other) const {
    return Complex(value / other, imaginary / other);
}

Complex Complex::operator/(const Complex& other) const {
    return operator*(other.conjugate()) / (other.value * other.value + other.imaginary * other.imaginary);
}

Complex Complex::operator^(const Complex& other) const {
    Complex logarithm = log(*this);

    Complex exponent = other * logarithm;
    NumericType abs = exponent.value;
    NumericType arg = exponent.imaginary;

    return log(abs) * Complex(std::tuple<NumericType, NumericType>(0, arg));
}

Complex Complex::operator+=(const Complex& other) {
    value += other.value;
    imaginary += other.imaginary;

    return *this;
}

Complex Complex::operator-=(const Complex& other) {
    value -= other.value;
    imaginary -= other.imaginary;

    return *this;
}

Complex Complex::operator*=(const Complex& other) {
    Complex prod = *this * other;

    value = prod.value;
    imaginary = prod.imaginary;
    return *this;
}

Complex Complex::operator/=(const NumericType& other) {
    value /= other;
    imaginary /= other;
    return *this;
}

Complex Complex::operator/=(const Complex& other) {
    Complex frac = *this / other;

    value = frac.value;
    imaginary = frac.imaginary;

    return *this;
}

Complex Complex::operator^=(const Complex& other) {
    Complex pow = *this ^ other;

    value = pow.value;
    imaginary = pow.imaginary;

    return *this;
}

Complex operator+(const NumericType& value, const Complex& other) {
    return Complex(value) + other;
}

Complex operator-(const NumericType& value, const Complex& other) {
    return Complex(value) - other;
}

Complex operator*(const NumericType& value, const Complex& other) {
    return Complex(value) * other;
}

Complex operator/(const NumericType& value, const Complex& other) {
    return Complex(value) / other;
}

Complex log(const Complex& c) {
    const auto& [abs, arg] = c.polarForm();

    return Complex(log(abs), arg);
}

Complex exp(const Complex& c) {
    const auto& [abs, arg] = c.polarForm();
    return abs * Complex(cos(arg), sin(arg));
}

Complex pow(const Complex& base, const Complex& exponent) {
    return exp(log(base) * exponent);
}
