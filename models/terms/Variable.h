//
// Created by SeanLi on 8/3/25.
//

#ifndef VARIABLE_H
#define VARIABLE_H

#include "../Terms.h"


class Variable final : public Term {
protected:
    std::unique_ptr<Type> type_buffer;

public:
    std::string name;

    explicit Variable(std::string name, std::unique_ptr<Type> type_buffer);
    ~Variable() override;

    [[nodiscard]] std::unique_ptr<Term> substitute(std::string target, Term& newValue) const override;;
    [[nodiscard]] std::unique_ptr<Term> alpha_convert(std::string newValue) const override;;
    [[nodiscard]] std::unique_ptr<Term> beta_reduce() const override;
    [[nodiscard]] std::unique_ptr<Term> clone() const override;
    [[nodiscard]] std::unique_ptr<Type> type_check(const TypingContext& context) const override;
    [[nodiscard]] std::unique_ptr<Type> get_type() const;
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] bool is_normal() const override;
    [[nodiscard]] bool has_free(std::string target) const override;
};

#endif //VARIABLE_H