#include <iostream>
#include <unordered_map>
#include <string>
#include <cmath>
#include <atomic>

class ExpressionFactory;

class Variable;

class Constant;

class Expression {
public:
    virtual double calculate(const std::unordered_map<std::string, double> &ctx) const = 0;

    virtual void free(ExpressionFactory *factory) = 0;
};

class ExpressionFactory {
    static std::unordered_map<std::string, Variable *> variables;
    static std::unordered_map<double, Constant *> constants;

    friend Variable;
    friend Constant;
public:
    ExpressionFactory();

    Variable *createVariable(std::string name);

    Constant *createConstant(double value);

    void free(Expression *exp) {
        exp->free(this);
    }
};

class Variable : public Expression {
protected:
    std::atomic<size_t> copiesN;
    const std::string name;

    Variable(std::string name = "")
            : copiesN{0}, name{name} {}

    ~Variable() {}

    friend ExpressionFactory;
public:
    double calculate(const std::unordered_map<std::string, double> &ctx) const override {
        auto it = ctx.find(name);
        if (it != ctx.end())
            return it->second;
        return 0;
    }

    void free(ExpressionFactory *factory) override {
        if (!factory)
            throw std::runtime_error("Bad pointer, unable to free memory.");

        copiesN--;
        if (copiesN != 0)
            return;

        factory->variables.erase(name);

        delete this;
    }
};

class Constant : public Variable {
protected:
    const bool is_reserved;
    const double value;

    Constant(double value, bool is_reserved = false)
            : value{value}, is_reserved{is_reserved} {}

    ~Constant() {}

    friend ExpressionFactory;
public:
    double calculate(const std::unordered_map<std::string, double> &ctx) const override {
        return value;
    }

    void free(ExpressionFactory *factory) override {
        if (is_reserved)
            return;
        if (!factory)
            throw std::runtime_error("Bad pointer, unable to free memory.");

        copiesN--;
        if (copiesN != 0)
            return;

        factory->constants.erase(value);

        delete this;
    }
};

std::unordered_map<std::string, Variable *> ExpressionFactory::variables;
std::unordered_map<double, Constant *> ExpressionFactory::constants;

ExpressionFactory::ExpressionFactory() {
    if (constants.empty()) {
        for (int i = -5; i < 257; i++)
            constants[i] = new Constant(i, true);
    }
}

Variable *ExpressionFactory::createVariable(std::string name) {
    auto it = variables.find(name);
    if (it != variables.end())
        return it->second;
    auto var = new Variable(name);
    variables[name] = var;
    return var;
}

Constant *ExpressionFactory::createConstant(double value) {
    auto it = constants.find(value);
    if (it != constants.end())
        return it->second;
    auto var = new Constant(value);
    constants[value] = var;
    return var;
}


class TwoOpExpression : public Expression {
protected:
    Expression *left;
    Expression *right;
public:
    TwoOpExpression() = delete;

    TwoOpExpression(Expression *left, Expression *right)
            : left{left}, right{right} {}

    ~TwoOpExpression() {
        auto factory = new ExpressionFactory;
        factory->free(left);
        factory->free(right);
        delete factory;
    }

    void free(ExpressionFactory *factory) override {
        delete this;
    }
};

class Addition : public TwoOpExpression {
public:
    using TwoOpExpression::TwoOpExpression;

    double calculate(const std::unordered_map<std::string, double> &ctx) const override {
        return left->calculate(ctx) + right->calculate(ctx);
    }
};

class Multiplication : public TwoOpExpression {
public:
    using TwoOpExpression::TwoOpExpression;

    double calculate(const std::unordered_map<std::string, double> &ctx) const override {
        return left->calculate(ctx) * right->calculate(ctx);
    }
};

class Dividing : public TwoOpExpression {
public:
    using TwoOpExpression::TwoOpExpression;

    double calculate(const std::unordered_map<std::string, double> &ctx) const override {
        return left->calculate(ctx) / right->calculate(ctx);
    }
};

class Subtraction : public TwoOpExpression {
public:
    using TwoOpExpression::TwoOpExpression;

    double calculate(const std::unordered_map<std::string, double> &ctx) const override {
        return left->calculate(ctx) - right->calculate(ctx);
    }
};


class Powering : public TwoOpExpression {
public:
    using TwoOpExpression::TwoOpExpression;

    double calculate(const std::unordered_map<std::string, double> &ctx) const override {
        return std::pow(left->calculate(ctx), right->calculate(ctx));
    }
};


int main() {
    while (true) {
        ExpressionFactory factory;
        auto c = factory.createConstant(2);
        auto x = factory.createVariable("x");
        auto add = new Addition(c, x);                                    //  x + 2
        auto mlp = new Multiplication(add, x);                            // (x + 2) * x
        std::unordered_map<std::string, double> ctx1 = {{"x", 3}};        // x = 3 => (x + 2) * x = 15
        std::unordered_map<std::string, double> ctx2 = {{"x", 4}};        // x = 4 => (x + 2) * x = 24

        std::cout << "x = 3 => (x + 2) * x = " << mlp->calculate(ctx1) << std::endl;
        std::cout << "x = 4 => (x + 2) * x = " << mlp->calculate(ctx2) << std::endl;
        delete mlp;
    }
    return 0;
}
