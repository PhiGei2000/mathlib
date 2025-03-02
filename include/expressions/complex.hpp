#pragma once
#include "number.hpp"

struct Complex : public Number {
    NumericType imaginary;

    inline constexpr Complex()
        : Number(0), imaginary(0) {
    }

    inline constexpr Complex(const NumericType& real)
        : Number(real), imaginary(0) {
    }

    inline constexpr Complex(const NumericType& real, const NumericType& imaginary)
        : Number(real), imaginary(imaginary) {
    }

    Complex(const std::tuple<NumericType, NumericType>& polarForm);

    virtual Number getValue() const override;

    Complex conjugate() const;
    std::tuple<NumericType, NumericType> polarForm() const;

    inline virtual bool isComplex() const override {
        return imaginary != 0;
    }

    virtual std::string to_string() const override;

    inline virtual Expression* copy() const override {
        return new Complex(value, imaginary);
    }

    Complex operator+(const Complex& other) const;
    Complex operator-() const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    Complex operator/(const NumericType& other) const;
    Complex operator/(const Complex& other) const;
    Complex operator^(const Complex& other) const;

    Complex operator+=(const Complex& other);
    Complex operator-=(const Complex& other);
    Complex operator*=(const Complex& other);
    Complex operator/=(const NumericType& other);
    Complex operator/=(const Complex& other);
    Complex operator^=(const Complex& other);
};

Complex operator+(const NumericType& value, const Complex& other);
Complex operator-(const NumericType& value, const Complex& other);
Complex operator*(const NumericType& value, const Complex& other);
Complex operator/(const NumericType& value, const Complex& other);

Complex log(const Complex& c);
Complex exp(const Complex& c);

static constexpr Complex I = Complex(0, 1);
