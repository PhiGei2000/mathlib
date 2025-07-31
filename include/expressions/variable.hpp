#pragma once
#include "expressions/expression.hpp"

#include <string>
#include <unordered_map>

struct Variable : public Expression{
    using SymbolType = std::string;
    const SymbolType symbol;

    Variable(const SymbolType& symbol);

    virtual Number getValue() const override;
    virtual std::string to_string() const override;
    virtual ExpressionTypes getType() const override;

    virtual std::set<Variable> getVariables() const override;

    virtual Expression* differentiate(const Variable* var) const override;

    virtual bool matches(const Expression* pattern) const override;

    virtual Expression* copy() const override {
        return new Variable(symbol);
    }

    bool operator==(const Variable& other) const;
    bool operator!=(const Variable& other) const;

};

namespace std {
    template<>
    struct less<Variable> {
        constexpr bool operator()(const Variable& lhs, const Variable& rhs) const {
            return std::less<Variable::SymbolType>{}(lhs.symbol, rhs.symbol);
        }
    };
}