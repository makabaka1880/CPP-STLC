//
// Created by SeanLi on 8/3/25.
//

#ifndef VARIABLE_H
#define VARIABLE_H

#include "../Terms.h"


class Variable final : public Term {
public:
    std::string name;

    explicit Variable(std::string name);
    ~Variable() override;

    [[nodiscard]] std::unique_ptr<Term> substitute(std::string target, Term& newValue) const override;;
    [[nodiscard]] std::unique_ptr<Term> alpha_convert(std::string newValue) const override;;
    [[nodiscard]] std::unique_ptr<Term> beta_reduce() const override;
    [[nodiscard]] std::unique_ptr<Term> clone() const override;
    [[nodiscard]] bool is_normal() const override;
    [[nodiscard]] bool has_free(std::string target) const override;
    [[nodiscard]] virtual std::string to_string() const = 0;
};

#endif //VARIABLE_H