#include "expressions/addition.hpp"

#include "expressions/expressions.hpp"

Expression* Addition::simplify() const {
    auto [simplifiedLeft, simplifiedRight] = simplifyChildren();

    switch (simplifiedLeft->getType()) {
        case ExpressionTypes::Number:
            return simplifyNummericalSummand(simplifiedLeft, simplifiedRight);
        default: break;
    }

    switch (simplifiedRight->getType()) {
        case ExpressionTypes::Number:
            return simplifyNummericalSummand(simplifiedRight, simplifiedLeft);
        default: break;
    }

    std::vector<const Expression*> summands;
    getSummands(simplifiedLeft, summands);
    getSummands(simplifiedRight, summands);

    // identify like terms and merge summands
    mergeSummands(summands);

    delete simplifiedLeft;
    delete simplifiedRight;
    return fromSummands(summands);

    // return new Addition(simplifiedLeft, simplifiedRight);
}

void Addition::mergeSummands(std::vector<const Expression*>& summands) {
    std::vector<std::pair<const Expression*, Number>> terms;

    Number numericalSummand = 0;
    for (auto it = summands.begin(); it != summands.end(); ) {
        if ((*it)->isNumeric()) {
            numericalSummand += (*it)->getValue();

            it = summands.erase(it);
            continue;
        }

        auto jt = terms.begin();
        for (; jt != terms.end(); jt++) {
            auto& [term, sum] = *jt;

            Number factor = 0;
            if (Expression::areLike(*it, term, factor)) {
                sum += factor;

                break;
            }
        }

        if (jt == terms.end()) {
            terms.emplace_back(*it, 1);
        }

        it = summands.erase(it);
    }

    if (numericalSummand != 0) {
        summands.push_back(new Number(numericalSummand));
    }

    for (auto [term, sum] : terms) {
        if (sum != 0) {
            Multiplication* mergedTerm = new Multiplication(new Number(sum), term->copy());
            Expression* simplified = mergedTerm->simplify();

            delete mergedTerm;
            summands.push_back(simplified);
        }
    }
}