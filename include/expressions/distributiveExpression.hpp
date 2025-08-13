#include "expressions/binaryExpression.hpp"

template<bool commutative, typename T, BinaryExpressionType TOver>
struct LeftDistributiveExpression : public virtual BinaryExpression<commutative> {
    inline virtual Expression* distribute() const {
        std::vector<const Expression*> rightTerms;

        if (right->getType() == getExpressionType<TOver>()) {
            reinterpret_cast<TOver*>(right)->getTerms();
        }
        else {
            rightTerms.push_back(right);
        }

        Expression* result = new T(left->copy(), rightTerms.back()->copy());
        for (auto it = rightTerms.rbegin() + 1; it != rightTerms.rend(); it++) {
            result = new TOver(result, new T(left->copy(), (*it)->copy()));
        }

        return result;
    }
};

template<bool commutative, typename T, BinaryExpressionType TOver>
struct RightDistributiveBinaryExpression : public virtual BinaryExpression<commutative> {

    inline virtual Expression* distribute() const {
        std::vector<const Expression*> leftTerms;

        if (left->getType() == getExpressionType<TOver>()) {
            reinterpret_cast<TOver*>(left)->getTerms(leftTerms);
        }
        else {
            leftTerms.push_back(left);
        }

        Expression* result = new T(leftTerms.back()->copy(), right->copy());
        for (auto it = leftTerms.rbegin() + 1; it != leftTerms.rend(); it++) {
            result = new TOver(result, new T((*it)->copy(), right->copy()));
        }

        return result;
    }
};

template<bool commutative, typename T, BinaryExpressionType TOver>
struct DistributiveBinaryExpression : public BinaryExpression<commutative> {
    inline constexpr DistributiveBinaryExpression() : BinaryExpression<commutative>() {
    }

    inline DistributiveBinaryExpression(Expression* left, Expression* right)
        : BinaryExpression<commutative>(left, right) {
    }

    inline virtual Expression* distribute() const {
        std::vector<const Expression*> leftTerms;
        std::vector<const Expression*> rightTerms;

        if (left->getType() == getExpressionType<TOver>()) {
            reinterpret_cast<TOver*>(left)->getTerms(leftTerms);
        }
        else {
            leftTerms.push_back(left);
        }

        if (right->getType() == getExpressionType<TOver>()) {
            reinterpret_cast<TOver*>(right)->getTerms(rightTerms);
        }
        else {
            rightTerms.push_back(right);
        }

        Expression* result = new T(leftTerms.front()->copy(), rightTerms.front()->copy());
        for (auto it = leftTerms.begin(); it != leftTerms.end(); it++) {
            auto jt = rightTerms.begin();
            if (it == leftTerms.begin()) {
                jt++;
            }

            for (; jt != rightTerms.end(); jt++) {
                result = new TOver(result, new T((*it)->copy(), (*jt)->copy()));
            }
        }

        return result;
    }
};