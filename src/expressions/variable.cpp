#include "expressions/variable.hpp"

#include "expressions/number.hpp"

#include <stdexcept>

Variable::Variable(const Variable::SymbolType& symbol)
    : symbol(symbol) {
}

Number Variable::getValue() const {
    throw std::runtime_error("this");
}

std::string Variable::toString() const {
    return std::string(symbol);
}

ExpressionTypes Variable::getType() const {
    return ExpressionTypes::Variable;
}

bool Variable::matches(const Expression* pattern) const {
    if (pattern->getType() != ExpressionTypes::Variable)
        return false;

    return true;
}