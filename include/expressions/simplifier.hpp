#pragma once
#include "expressions/binaryExpression.hpp"

class Simplifier {
  public:
    template<CommutativeBinaryExpressionType T>
    static Expression* simplify(const T* expr, const std::function<Number(const Number&, const Number&)>& operation) {
        auto [simplifiedLeft, simplifiedRight] = expr->simplifyChildren();

        Expression* simplified;

        if (simplifiedLeft->getType() == ExpressionTypes::Number || simplifiedRight->getType() == ExpressionTypes::Number) {
            simplified = T(simplifiedLeft, simplifiedRight).simplifyNumericalSubexpression(operation);

            delete simplifiedLeft;
            delete simplifiedRight;
        }
        else {
            simplified = new T(simplifiedLeft, simplifiedRight);
        }

        Expression* result;
        if (simplified->getType() == getExpressionType<T>()) {
            std::vector<const Expression*> terms;
            simplified->getTerms(terms);

            mergeTerms<T>(terms);
            result = fromTerms<T>(terms);
        }
        else {
            result = simplified->simplify();
        }
        delete simplified;

        return result;
    }

    template<CommutativeBinaryExpressionType T>
    static Expression* fromTerms(const std::vector<const Expression*>& terms) {
        if (terms.size() == 0) {
            std::optional<Number> identity = T().getIdentity();
            if (identity.has_value()) {
                return new Number(identity.value());
            }
            else {
                return nullptr;
            }
        }

        Expression* result = terms.front()->copy();
        delete terms.front();

        for (int i = 1; i < terms.size(); i++) {
            result = new T(result, terms[i]->copy());
            delete terms[i];
        }

        return result;
    }

    template<CommutativeBinaryExpressionType T>
    static void mergeTerms(std::vector<const Expression*>& terms) {
        for (auto it = terms.begin(); it != terms.end();) {
            it = terms.insert(it, (*it)->copy());
        }
    }
};