//
// Created by SeanLi on 8/3/25.
//

#ifndef ABSTRACTION_H
#define ABSTRACTION_H

#include "Variable.h"
#include "../Terms.h"

#include <string>
#include <memory>

class Abstraction final : public Term {
public:
    std::unique_ptr<Variable> variable;
    std::unique_ptr<Term> body;

    explicit Abstraction(std::unique_ptr<Variable> variable, std::unique_ptr<Term> body);
    ~Abstraction() override;;

    [[nodiscard]] std::unique_ptr<Term> alpha_convert(std::string newValue) const override;;
    [[nodiscard]] std::unique_ptr<Term> substitute(std::string target, Term& newValue) const override;;
    [[nodiscard]] std::unique_ptr<Term> clone() const override;
    [[nodiscard]] std::unique_ptr<Term> beta_reduce() const override;
    [[nodiscard]] Type& type() const override;
    [[nodiscard]] bool is_normal() const override;
    [[nodiscard]] bool has_free(std::string target) const override;
    [[nodiscard]] virtual std::string to_string() const override;
};
#endif //ABSTRACTION_H
