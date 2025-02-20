#include "expressions/variable.hpp"

#include "expressions/number.hpp"

#include <stdexcept>

Variable::Variable(const Variable::SymbolType& symbol)
    : symbol(symbol) {
}

Expression* Variable::differentiate(const Variable* var) const {
    if (var->symbol == this->symbol) {
        return new Number(1);
    }

    return new Number(0);
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