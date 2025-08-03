//
// Created by SeanLi on 8/3/25.
//

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include "../models/Terms.h"
#include <stdexcept>
#include <string>
#include <sstream>

class ReductionOnNormalForm: public std::runtime_error {
private:
    std::string fullMessage;
public:
    explicit ReductionOnNormalForm(const std::unique_ptr<Term> &term):
        std::runtime_error("")
    {
        std::stringstream ss;
        ss << "Reduction on normal form " << term->to_string();
        fullMessage = ss.str();
    }

    [[nodiscard]] const char* what() const noexcept override;
};

class UndeclaredVariableError : public std::runtime_error {
private:
    std::string fullMessage;
public:
    explicit UndeclaredVariableError(const std::string& name):
        std::runtime_error("")
    {
        std::stringstream ss;
        ss << "Undeclared variable: '" << name << "'";
        fullMessage = ss.str();
    }

    [[nodiscard]] const char* what() const noexcept override;
};
#endif //EXCEPTIONS_H
