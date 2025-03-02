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
    if (left->getType() == ExpressionTypes::Addition || right->getType() == ExpressionTypes::Addition) {
        simplifyExpandable();
    }

    // simplify both factors
    auto [simplifiedLeft, simplifiedRight] = simplifyChildren();

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
    std::vector<const Expression*> factors;
    getFactors(simplifiedLeft, factors);
    getFactors(simplifiedRight, factors);

    Number numericalFactor = 1;
    std::vector<const Expression*> mergedFactors;
    mergeFactors(numericalFactor, factors, mergedFactors);

    // create simplified expression
    Expression* result = nullptr;
    if (factors.size() > 0) {
        result = fromFactors(factors);
    }

    if (mergedFactors.size() > 0) {
        result = result != nullptr ? new Multiplication(result, fromFactors(mergedFactors)) : fromFactors(mergedFactors);

        for (auto it = mergedFactors.begin(); it != mergedFactors.end(); it++) {
            delete *it;
        }
    }

    if (numericalFactor != 1) {
        result = new Multiplication(new Number(numericalFactor), result);
    }

    delete simplifiedLeft;
    delete simplifiedRight;

    return result;
}

Expression* Multiplication::simplifyExpandable() const {
    Expression* expanded = expand();

    Expression* result = expanded->simplify();
    delete expanded;

    return result;
}

void Multiplication::mergeFactors(Number& numericalFactor, std::vector<const Expression*>& factors, std::vector<const Expression*>& mergedFactors) {
    std::unordered_map<Variable::SymbolType, Expression*> variables;

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
                    mergedFactors.push_back(new Variable(symbol));
                    delete simplifiedExponent;
                    break;
                }
            }
            default:
                mergedFactors.push_back(new Exponentiation(new Variable(symbol), simplifiedExponent));
                break;
        }

        delete exponent;
    }
}

Expression* Multiplication::simplifyNumericalFactor(Expression* number, Expression* other) {
    Number value = number->getValue();
    delete number;

    if (value == 0) {
        delete other;
        return new Number(0);
    }
    else if (value == 1) {
        return other;
    }
    else
        switch (other->getType()) {
            case ExpressionTypes::Number: {
                Number product = value * other->getValue();
                delete other;

                return new Number(product);
            }
            case ExpressionTypes::Multiplication: {
                std::vector<const Expression*> factors;
                getFactors(other, factors);

                Number result = value;
                auto it = factors.begin();

                while (it != factors.end()) {
                    if ((*it)->getType() == ExpressionTypes::Number) {
                        result *= (*it)->getValue();

                        it = factors.erase(it);
                    }
                    else {
                        it++;
                    }
                }

                Expression* nonNumericalFactors = fromFactors(factors);
                delete other;

                return new Multiplication(new Number(result), nonNumericalFactors);
            }
            default: break;
        }

    return new Multiplication(new Number(value), other);
}