#include "expressions/multiplication.hpp"

#include "expressions/addition.hpp"
#include "expressions/exponentiation.hpp"

#include <stack>

Expression* Multiplication::expand() const {
    std::vector<const Expression*> leftSummands;
    std::vector<const Expression*> rightSummands;

    Expression* left = this->left->expand();
    Expression* right = this->right->expand();

    getSummands(left, leftSummands);
    getSummands(right, rightSummands);

    if (leftSummands.size() == 1 && rightSummands.size() == 1) {
        delete left;
        delete right;

        return copy();
    }

    // (a + b) * (c + d) = ac + ad + bc + bd
    Expression* result = nullptr;
    for (int l = leftSummands.size() - 1; l >= 0; l--) {
        for (int r = rightSummands.size() - 1; r >= 0; r--) {
            if (result == nullptr) {
                result = new Multiplication(leftSummands[l]->copy(), rightSummands[r]->copy());
            }
            else {
                result = new Addition(result, new Multiplication(leftSummands[l]->copy(), rightSummands[r]->copy()));
            }
        }
    }

    delete left;
    delete right;

    return result;
}

Expression* Multiplication::simplify() const {
    // simplify both factors
    Expression* expandedLeft = left->expand();
    Expression* simplifiedLeft = expandedLeft->simplify();
    delete expandedLeft;

    Expression* expandedRight = right->expand();
    Expression* simplifiedRight = expandedRight->simplify();
    delete expandedRight;

    // simplify numerical factors
    switch (simplifiedLeft->getType()) {
        case ExpressionTypes::Number:
            return simplifyNumericalFactor(simplifiedLeft, simplifiedRight);
        default: break;
    }

    switch (simplifiedRight->getType()) {
        case ExpressionTypes::Number:
            return simplifyNumericalFactor(simplifiedRight, simplifiedLeft);
        default: break;
    }

    // merge like terms
    Number numericalFactor = 1;
    std::unordered_map<Variable::SymbolType, Expression*> variables;

    std::vector<const Expression*> factors;
    getFactors(simplifiedLeft, factors);
    getFactors(simplifiedRight, factors);

    for (auto it = factors.begin(); it != factors.end();) {
        switch ((*it)->getType()) {
            case ExpressionTypes::Number:
                numericalFactor *= (*it)->getValue();

                it = factors.erase(it);
                break;
            case ExpressionTypes::Variable: {
                const Variable* var = static_cast<const Variable*>(*it);

                if (variables.contains(var->symbol)) {
                    variables[var->symbol] = new Addition(variables[var->symbol], new Number(1));
                }
                else {
                    variables[var->symbol] = new Number(1);
                }

                it = factors.erase(it);
            } break;
            case ExpressionTypes::Exponentiation: {
                const Exponentiation* exp = static_cast<const Exponentiation*>(*it);
                const Expression* base = exp->left;

                if (base->getType() == ExpressionTypes::Variable) {
                    const Variable* baseVar = static_cast<const Variable*>(base);

                    if (variables.contains(baseVar->symbol)) {
                        variables[baseVar->symbol] = new Addition(variables[baseVar->symbol], exp->right->copy());
                    }
                    else {
                        variables.insert(std::make_pair(baseVar->symbol, exp->right->copy()));
                    }

                    it = factors.erase(it);
                }
                else {
                    it++;
                }
            } break;
            default:
                it++;
        }
    }

    // sum up exponents
    for (const auto [symbol, exponent] : variables) {
        Expression* simplifiedExponent = exponent->simplify();
        switch (simplifiedExponent->getType()) {
            case ExpressionTypes::Number: {
                Number value = simplifiedExponent->getValue();

                if (value == 1) {
                    factors.push_back(new Variable(symbol));
                    delete simplifiedExponent;
                    break;
                }
            }
            default:
                factors.push_back(new Exponentiation(new Variable(symbol), simplifiedExponent));
                break;
        }

        delete exponent;
    }

    // create simplified expression
    Expression* result = new Multiplication(new Number(numericalFactor), fromFactors(factors));
    delete simplifiedLeft;
    delete simplifiedRight;

    return result;
}
