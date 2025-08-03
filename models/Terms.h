//
// Created by SeanLi on 8/3/25.
//

#ifndef TERM_H
#define TERM_H

#include <string>
#include <memory>

class Term {
public:
    virtual ~Term() = 0;
    [[nodiscard]] virtual std::unique_ptr<Term> alpha_convert(std::string newValue) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Term> substitute(std::string target, Term& newValue) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Term> beta_reduce() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Term> clone() const = 0;
    [[nodiscard]] virtual bool is_normal() const = 0;
    [[nodiscard]] virtual bool has_free(std::string target) const = 0;
    [[nodiscard]] virtual std::string to_string() const = 0;
};

std::ostream& operator<<(std::ostream& os, const Term& term);
#endif //TERM_H