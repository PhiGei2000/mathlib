#pragma once
#include "expressions/expression.hpp"

#include "expressions/number.hpp"
#include "expressions/variable.hpp"

#include <algorithm>
#include <concepts>
#include <functional>

struct BinaryExpressionBase : public Expression {
    Expression* left = nullptr;
    Expression* right = nullptr;

    inline constexpr BinaryExpressionBase() {
    }

    inline BinaryExpressionBase(Expression* left, Expression* right)
        : left(left), right(right) {
        this->left->parent = this;
        this->right->parent = this;
    }

    inline virtual bool isBinary() const override {
        return true;
    }

    inline virtual bool isNumeric() const override {
        return left->isNumeric() && right->isNumeric();
    }

    inline virtual std::vector<const Expression*> getChildren() const override {
        return {left, right};
    }

    inline virtual std::set<Variable> getVariables() const override {
        std::set<Variable> leftVariables = left->getVariables();
        const std::set<Variable>& rightVariables = right->getVariables();

        leftVariables.insert_range(rightVariables);
        return leftVariables;
    }

    inline virtual void replaceChild(Expression* child, Expression* expr) override {
        if (left == child) {
            left->parent = nullptr;
            left = expr;
            left->parent = this;
        }
        else if (right == child) {
            right->parent = nullptr;
            right = expr;
            right->parent = this;
        }
        else {
            left->replaceChild(child, expr);
            right->replaceChild(child, expr);
        }
    }

    inline std::pair<Expression*, Expression*> simplifyChildren() const {
        Expression* simplifiedLeft = left->simplify();
        Expression* simplifiedRight = right->simplify();

        return {simplifiedLeft, simplifiedRight};
    }

    inline virtual void getTerms(std::vector<const Expression*>& terms) const override {
        if (left->getType() == getType()) {
            left->getTerms(terms);
        }
        else {
            terms.push_back(left);
        }

        if (right->getType() == getType()) {
            right->getTerms(terms);
        }
        else {
            terms.push_back(right);
        }
    }

    inline virtual constexpr std::optional<Number> getLeftIdentity() const {
        return {};
    }

    inline virtual constexpr std::optional<Number> getRightIdentity() const {
        return {};
    }

    inline virtual constexpr std::optional<Number> getAbsorbingElement() const {
        return {};
    }

  protected:
    virtual Expression* simplifyNumericalSubexpression(const std::function<Number(const Number&, const Number&)>& operation) const {
        if (left->getType() == ExpressionTypes::Number) {
            Number value = left->getValue();

            if (value == getLeftIdentity()) {
                return right->copy();
            }
            else if (value == getAbsorbingElement()) {
                return new Number(getAbsorbingElement().value());
            }
            else if (right->getType() == ExpressionTypes::Number) {
                Number value2 = right->getValue();

                return new Number(operation(value, value2));
            }
        }
        else if (right->getType() == ExpressionTypes::Number) {
            Number value = right->getValue();

            if (value == getRightIdentity()) {
                return right->copy();
            }
            else if (value == getAbsorbingElement()) {
                return new Number(getAbsorbingElement().value());
            }
        }

        return copy();
    }
};

template<bool commutative>
struct BinaryExpression : public BinaryExpressionBase {
    inline constexpr BinaryExpression() : BinaryExpressionBase() {
    }

    inline BinaryExpression(Expression* left, Expression* right)
        : BinaryExpressionBase(left, right) {
    }

    inline virtual bool matches(const Expression* pattern) const override {
        switch (pattern->getType()) {
            case ExpressionTypes::Variable:
                return true;
            default: {
                bool sameType = (getType() == pattern->getType());

                if (!sameType)
                    return false;

                const BinaryExpression* binaryPattern = reinterpret_cast<const BinaryExpression*>(pattern);
                bool leftMatch = left->matches(binaryPattern->left);
                bool rightMatch = right->matches(binaryPattern->right);
                if (leftMatch && rightMatch)
                    return true;
            }
        }

        return false;
    }
};

template<>
struct BinaryExpression<true> : public BinaryExpressionBase {
    inline constexpr BinaryExpression() : BinaryExpressionBase() {
    }

    inline BinaryExpression(Expression* left, Expression* right)
        : BinaryExpressionBase(left, right) {
    }

    inline virtual constexpr std::optional<Number> getIdentity() const {
        return {};
    };

    inline virtual constexpr std::optional<Number> getLeftIdentity() const override {
        return getIdentity();
    }

    inline virtual constexpr std::optional<Number> getRightIdentity() const override {
        return getIdentity();
    }

    inline virtual bool matches(const Expression* pattern) const override {
        switch (pattern->getType()) {
            case ExpressionTypes::Variable:
                return true;
            default: {
                bool sameType = (getType() == pattern->getType());

                if (!sameType)
                    return false;

                const BinaryExpression* binaryPattern = reinterpret_cast<const BinaryExpression*>(pattern);
                bool leftMatch = left->matches(binaryPattern->left);
                bool rightMatch = right->matches(binaryPattern->right);
                if (leftMatch && rightMatch)
                    return true;

                leftMatch = left->matches(binaryPattern->right);
                rightMatch = right->matches(binaryPattern->left);

                return leftMatch && rightMatch;
            }
        }

        return false;
    }

    inline virtual Expression* simplifyNumericalSubexpression(const std::function<Number(const Number&, const Number&)>& operation) const override {
        Expression* numerical;
        Expression* other;

        if (left->getType() == ExpressionTypes::Number) {
            numerical = left;
            other = right;
        }
        else {
            numerical = right;
            other = left;
        }

        Number value = numerical->getValue();

        if (value == getIdentity()) {
            return other->copy();
        }
        else if (value == getAbsorbingElement()) {
            return new Number(getAbsorbingElement().value());
        }
        else if (other->getType() == ExpressionTypes::Number) {
            Number value2 = other->getValue();

            return new Number(operation(value, value2));
        }

        return copy();
    }
};

template<typename T>
concept BinaryExpressionType = std::is_base_of_v<BinaryExpressionBase, T> && requires(Expression* left, Expression* right) {
    T(left, right);
};

template<typename T>
concept CommutativeBinaryExpressionType = std::is_base_of_v<BinaryExpression<true>, T> && requires(Expression* left, Expression* right) {
    T(left, right);
};
