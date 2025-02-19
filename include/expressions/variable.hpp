#pragma once
#include "expressions/expression.hpp"

#include <string>
#include <unordered_map>

struct Variable : public Expression{
    using SymbolType = std::string;
    const SymbolType symbol;

    Variable(const SymbolType& symbol);

    virtual Number getValue() const override;
    virtual std::string toString() const override;
    virtual ExpressionTypes getType() const override;

    virtual bool matches(const Expression* pattern) const override;

    virtual Expression* copy() const override {
        return new Variable(symbol);
    }
};