#pragma once
#include "expressions/expression.hpp"

struct BinaryExpressionBase : public Expression {
    Expression* left;
    Expression* right;

    inline BinaryExpressionBase(Expression* left, Expression* right)
        : left(left), right(right) {
        this->left->parent = this;
        this->right->parent = this;
    }

    inline virtual bool isBinary() const override {
        return true;
    }

    inline virtual std::vector<const Expression*> getChildren() const override {
        return {left, right};
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
        Expression* expandedLeft = left->expand();
        Expression* simplifiedLeft = expandedLeft->simplify();
        delete expandedLeft;

        Expression* expandedRight = right->expand();
        Expression* simplifiedRight = expandedRight->simplify();
        delete expandedRight;

        return {simplifiedLeft, simplifiedRight};
    }
};

template<bool commutative>
struct BinaryExpression : public BinaryExpressionBase {
    BinaryExpression(Expression* left, Expression* right)
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

                if (commutative) {
                    leftMatch = left->matches(binaryPattern->right);
                    rightMatch = right->matches(binaryPattern->left);

                    return leftMatch && rightMatch;
                }
            }
        }

        return false;
    }
};