//
// Created by SeanLi on 8/3/25.
//

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include "../models/Terms.h"
#include <stdexcept>
#include <string>
#include <sstream>

class ReductionOnNormalForm final : public std::runtime_error {
private:
    std::string fullMessage;
public:
    explicit ReductionOnNormalForm(const std::unique_ptr<Term> &term):
        std::runtime_error("")
    {
        std::stringstream ss;
        ss << "Reduction on normal form: " << term->to_string();
        fullMessage = ss.str();
    }

    [[nodiscard]] const char* what() const noexcept override;
};

class UndeclaredVariableError final : public std::runtime_error {
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

class TypeMismatchError : public std::runtime_error {
public:
    explicit TypeMismatchError(const std::string& message)
        : std::runtime_error(message) {}
};

class NotAFunctionError final : public TypeMismatchError {
public:
    explicit NotAFunctionError(const std::string& typeName)
        : TypeMismatchError("Type error: '" + typeName + "' is not a function.") {}
};

class DomainTypeMismatchError final : public TypeMismatchError {
public:
    explicit DomainTypeMismatchError(const std::string& expected, const std::string& actual)
        : TypeMismatchError("Type mismatch: expecting domain '" + expected + "', got '" + actual + "'") {}
};

#endif //EXCEPTIONS_H
