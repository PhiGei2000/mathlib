#include "expressions/multiplication.hpp"

#include "expressions/expressions.hpp"
#include "expressions/simplifier.hpp"

#include <stack>

Expression* Multiplication::simplify() const {
    if (left->getType() == ExpressionTypes::Addition || right->getType() == ExpressionTypes::Addition) {
        Expression* expanded = distribute();
        Expression* result = expanded->simplify();

        delete expanded;
        return result;
    }

    return Simplifier::simplify<Multiplication>(this, [](const Number& x, const Number& y) { return x * y; });
}

template<>
void Simplifier::mergeTerms<Multiplication>(std::vector<const Expression*>& factors) {
    std::unordered_map<Variable::SymbolType, Expression*> variables;

    Number numericalFactor = 1;
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

    std::vector<const Expression*> mergedFactors;
    if (numericalFactor != 1) {
        mergedFactors.push_back(new Number(numericalFactor));
    }

    for (auto it = factors.begin(); it != factors.end();) {
        mergedFactors.push_back((*it)->copy());
        it = factors.erase(it);
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
            default: {
                Expression* mergedFactor = new Exponentiation(new Variable(symbol), simplifiedExponent);
                Expression* simplifiedFactor = mergedFactor->simplify();

                if (simplifiedFactor->isNumeric()) {
                    numericalFactor *= simplifiedFactor->getValue();
                    delete simplifiedFactor;
                }
                else {
                    mergedFactors.push_back(simplifiedFactor);
                }
                delete mergedFactor;
            } break;
        }

        delete exponent;
    }

    factors.append_range(mergedFactors);
}
