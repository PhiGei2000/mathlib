#include "expressions/addition.hpp"

#include "expressions/expressions.hpp"
#include "expressions/simplifier.hpp"

Expression* Addition::simplify() const {
    return Simplifier::simplify(this, [](const Number& x, const Number& y) { return x + y; });
}

template<>
void Simplifier::mergeTerms<Addition>(std::vector<const Expression*>& summands) {

    std::vector<std::pair<const Expression*, Number>> terms;

    Number numericalSummand = 0;
    for (auto it = summands.begin(); it != summands.end();) {
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