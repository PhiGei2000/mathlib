#include "expressions/addition.hpp"
#include "expressions/complex.hpp"
#include "expressions/exponentiation.hpp"
#include "expressions/expression.hpp"
#include "expressions/multiplication.hpp"
#include "expressions/number.hpp"
#include "expressions/variable.hpp"

template<ExpressionType T>
inline Expression* copyExpression(const T* expr) {
    return expr->copy();
}

template<ExpressionType T>
inline Expression* copyExpression(T* expr) {
    return expr;
}

template<ExpressionType T>
inline Expression* copyExpression(const T& expr) {
    return expr.copy();
}

template<typename T1, typename T2>
inline Addition* add(const T1 first, const T2 second) {
    return new Addition(copyExpression(first), copyExpression(second));
}

template<typename T1, typename T2>
inline Addition* sub(const T1 first, const T2 second) {
    return new Addition(copyExpression(first), new Multiplication(new Number(-1), copyExpression(second)));
}

template<typename T1, typename T2>
inline Multiplication* mul(const T1 first, const T2 second) {
    return new Multiplication(copyExpression(first), copyExpression(second));
}

template<typename T1, typename T2>
inline Multiplication* div(const T1 first, const T2 second) {
    return new Multiplication(copyExpression(first), new Exponentiation(copyExpression(second), new Number(-1)));
}

template<typename T1, typename T2>
inline Exponentiation* pow(const T1 first, const T2 second) {
    return new Exponentiation(copyExpression(first), copyExpression(second));
}
