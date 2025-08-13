#pragma once
#include <concepts>
#include <set>
#include <string>
#include <vector>

struct Number;
struct Expression;
struct Variable;

enum class ExpressionTypes : unsigned int {
    Number = 1,
    Variable = 2,
    Exponentiation = 3,
    Multiplication = 4,
    Addition = 5
};

template<typename T>
concept ExpressionType = std::is_base_of_v<Expression, T>;

template<ExpressionType T>
ExpressionTypes getExpressionType();

struct Expression {
  public:
    static int expressionCount;
    static std::vector<const Expression*> expressions;

    Expression* parent = nullptr;

    virtual ~Expression() = default;

    virtual Number getValue() const = 0;

    virtual std::string to_string() const = 0;

    virtual ExpressionTypes getType() const = 0;

    inline virtual bool isBinary() const {
        return false;
    }

    inline virtual bool isNumeric() const {
        return false;
    }

    inline virtual bool isComplex() const {
        return false;
    }

    inline virtual std::vector<const Expression*> getChildren() const {
        return {};
    }

    virtual std::set<Variable> getVariables() const;

    static bool areLike(const Expression* first, const Expression* second, Number& factor);

    inline virtual bool matches(const Expression* pattern) const = 0;

    virtual Expression* differentiate(const Variable* var) const;

    inline virtual Expression* simplify() const {
        return copy();
    }

    inline virtual void replaceChild(Expression* child, Expression* expr) {
    }

    virtual Expression* copy() const = 0;

    template<ExpressionType T>
    inline T& get() {
        return dynamic_cast<T>(*this);
    }

    template<ExpressionType T>
    inline const T& get() const {
        return dynamic_cast<T>(*this);
    }

    inline static void* operator new(std::size_t s) {
        expressionCount += 1;

        void* ptr = ::operator new(s);
        expressions.push_back(static_cast<const Expression*>(ptr));

        return ptr;
    }

    static void operator delete(void* ptr);

    inline virtual void getTerms(std::vector<const Expression*>& terms) const {
        terms.push_back(this);
    }
};
