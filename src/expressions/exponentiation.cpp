#include "expressions/exponentiation.hpp"

Expression* Exponentiation::expand() const {
    std::vector<const Expression*> leftFactors;

    Expression* left = this->left->expand();
    getFactors(left, leftFactors);

    if (leftFactors.size() == 1) {
        delete left;

        return copy();
    }

    // (a * b) ^ c = a ^ c * b ^ c
    Expression* result = new Exponentiation(leftFactors[leftFactors.size() - 1]->copy(), right->copy());
    for (int l = leftFactors.size() - 2; l >= 0; l--) {
        result = new Multiplication(result, new Exponentiation(leftFactors[l]->copy(), right->copy()));
    }

    delete left;

    return result;
}