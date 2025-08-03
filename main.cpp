#include <iostream>
#include "lambda.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto s_term = std::make_unique<Abstraction>(
    std::make_unique<Variable>("x"),
    std::make_unique<Abstraction>(
        std::make_unique<Variable>("y"),
        std::make_unique<Abstraction>(
            std::make_unique<Variable>("z"),
            std::make_unique<Application>(
                std::make_unique<Application>(
                    std::make_unique<Variable>("x"),
                    std::make_unique<Variable>("z")),
                std::make_unique<Application>(
                    std::make_unique<Variable>("y"),
                    std::make_unique<Variable>("z"))
            ))));

    // Identity combinator
    auto i_combinator_a = std::make_unique<Abstraction>(std::make_unique<Variable>("a"), std::make_unique<Variable>("a"));
    auto i_combinator_b = std::make_unique<Abstraction>(std::make_unique<Variable>("b"), std::make_unique<Variable>("b"));

    // ((S I) I)
    auto test_term = std::make_unique<Application>(
        std::make_unique<Application>(std::move(s_term), std::move(i_combinator_a)),
        std::move(i_combinator_b));

    // The self-applying function
    auto body = std::make_unique<Application>(std::make_unique<Variable>("x"), std::make_unique<Variable>("x"));
    auto omega_body = std::make_unique<Abstraction>(std::make_unique<Variable>("x"), std::move(body));

    // The full omega term
    auto omega = std::make_unique<Application>(
        std::move(omega_body->clone()),
        std::move(omega_body));

    std::cout << *test_term << std::endl << *omega << std::endl;
    return 0;
}