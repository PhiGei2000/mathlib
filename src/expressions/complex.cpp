#include "expressions/complex.hpp"

#include <cmath>

#include <numbers>

Complex::Complex(const std::tuple<double, double>& polarForm)
    : Number(std::get<0>(polarForm) * cos(std::get<1>(polarForm))), imaginary(std::get<0>(polarForm) * sin(std::get<1>(polarForm))) {
}

Number Complex::getValue() const {
    return Complex(*this);
}

Complex Complex::conjugate() const {
    return Complex(value, -imaginary);
}

std::tuple<double, double> Complex::polarForm() const {
    double abs = sqrt(value * value + imaginary * imaginary);
    double arg = value == 0 ? (imaginary < 0 ? -1 : 1) * std::numbers::pi / 2 : atan2(imaginary, value);

    return std::tuple<double, double>(abs, arg);
}

std::string Complex::toString() const {
    if (imaginary == 0) {
        return Number::toString();
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

Complex Complex::operator/(const double& other) const {
    return Complex(value / other, imaginary / other);
}

Complex Complex::operator/(const Complex& other) const {
    return operator*(other.conjugate()) / (other.value * other.value + other.imaginary * other.imaginary);
}

Complex Complex::operator^(const Complex& other) const {
    Complex logarithm = log(*this);

    Complex exponent = other * logarithm;
    double abs = exponent.value;
    double arg = exponent.imaginary;

    return log(abs) * Complex(std::tuple<double, double>(0, arg));
}

Complex operator+(const double& value, const Complex& other) {
    return Complex(value) + other;
}

Complex operator-(const double& value, const Complex& other) {
    return Complex(value) - other;
}

Complex operator*(const double& value, const Complex& other) {
    return Complex(value) * other;
}

Complex operator/(const double& value, const Complex& other) {
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
