#pragma once
#include "number.hpp"

struct Complex : public Number {
    double imaginary;

    inline constexpr Complex()
        : Number(0), imaginary(0) {
    }

    inline constexpr Complex(const double& real)
        : Number(real), imaginary(0) {
    }

    inline constexpr Complex(double real, double imaginary)
        : Number(real), imaginary(imaginary) {
    }

    Complex(const std::tuple<double, double>& polarForm);

    virtual Number getValue() const override;

    Complex conjugate() const;
    std::tuple<double, double> polarForm() const;

    virtual std::string toString() const override;

    inline virtual Expression* copy() const override {
        return new Complex(value, imaginary);
    }

    Complex operator+(const Complex& other) const;
    Complex operator-() const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    Complex operator/(const double& other) const;
    Complex operator/(const Complex& other) const;
    Complex operator^(const Complex& other) const;
};

Complex operator+(const double& value, const Complex& other);
Complex operator-(const double& value, const Complex& other);
Complex operator*(const double& value, const Complex& other);
Complex operator/(const double& value, const Complex& other);

Complex log(const Complex& c);
Complex exp(const Complex& c);

static constexpr Complex I = Complex(0, 1);
