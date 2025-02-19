#include "expressions/multiplication.hpp"

#include "expressions/addition.hpp"

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

