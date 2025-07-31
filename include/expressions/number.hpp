#pragma once
#include "expressions/expression.hpp"
#include <cmath>

using NumericType = double;

struct Number : public Expression {
  protected:
    NumericType value;

  public:
    inline constexpr Number(const NumericType& value)
        : value(value) {
    }

    virtual Number getValue() const override;
    virtual std::string to_string() const override;
    virtual ExpressionTypes getType() const override;

    inline virtual bool isNumeric() const override {
        return true;
    }

    inline bool isInteger() const {
        return std::floor(value) == value;
    }

    virtual bool matches(const Expression* pattern) const override;

    inline virtual Expression* copy() const override {
        return new Number(value);
    }

    Number operator+(const Number& other) const;
    Number operator-() const;
    Number operator-(const Number& other) const;
    Number operator*(const Number& other) const;
    Number operator/(const Number& other) const;
    Number operator^(const Number& exponent) const;

    inline Number operator+=(const Number& other) {
        value += other.value;
        return *this;
    }

    inline Number operator-=(const Number& other) {
        value -= other.value;
        return *this;
    }

    inline Number operator*=(const Number& other) {
        value *= other.value;
        return *this;
    }

    inline Number operator/=(const Number& other) {
        value /= other.value;
        return *this;
    }

    operator double() const;
};
