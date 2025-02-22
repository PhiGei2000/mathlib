#pragma once
#include "expressions/expression.hpp"

struct Number : public Expression {
  protected:
    double value;

  public:
    inline constexpr Number(double value)
        : value(value) {
    }

    virtual Number getValue() const override;
    virtual std::string toString() const override;
    virtual ExpressionTypes getType() const override;

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
