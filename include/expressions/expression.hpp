#pragma once
#include <concepts>
#include <set>
#include <string>
#include <vector>

struct Number;
struct Expression;

enum class ExpressionTypes : unsigned int {
    Number = 1,
    Variable = 2,
    Exponentiation = 3,
    Multiplication = 4,
    Addition = 5
};

template<typename T>
concept ExpressionType = std::is_base_of<Expression, T>::value && !std::is_abstract<T>::value;

struct Expression {
  public:
    static int expressionCount;

    Expression* parent = nullptr;

    virtual ~Expression() = default;

    virtual Number getValue() const = 0;

    virtual std::string toString() const = 0;

    virtual ExpressionTypes getType() const = 0;

    inline virtual bool isBinary() const {
        return false;
    }

    inline virtual std::vector<const Expression*> getChildren() const {
        return {};
    }

    inline virtual bool matches(const Expression* pattern) const = 0;

    inline virtual Expression* expand() const {
        return copy();
    };

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

        return ::operator new(s);
    }

    static void operator delete(void* ptr);

  protected:
    static void getSummands(const Expression* expr, std::vector<const Expression*>& summands);
    static void getFactors(const Expression* expr, std::vector<const Expression*>& factors);
};

