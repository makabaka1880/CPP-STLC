//
// Created by SeanLi on 8/3/25.
//

#include "Abstraction.h"

#include "../Terms.h"
#include "../../utils.h"
#include "../../exceptions/Exceptions.h"

using std::unique_ptr; using std::make_unique;

Abstraction::Abstraction(unique_ptr<Variable> variable, unique_ptr<Term> body):
    variable(std::move(variable)),
    body(std::move(body))
{}

Abstraction::~Abstraction() = default;

unique_ptr<Term> Abstraction::alpha_convert(const std::string newValue) const {
    auto new_variable = make_unique<Variable>(newValue);
    auto new_body = this->body->substitute(this->variable->name, *new_variable);
    return make_unique<Abstraction>(
        std::move(new_variable),
        std::move(new_body)
    );
}

unique_ptr<Term> Abstraction::substitute(std::string target, Term& newValue) const {
    if (this->variable->name == target) {
        return this->clone();
    }
    auto this_var = make_unique<Variable>(this->variable->name);
    return make_unique<Abstraction>(
        std::move(this_var),
        this->body->substitute(target, newValue)
    );
}

unique_ptr<Term> Abstraction::clone() const {
    auto cloned_variable_term = this->variable->clone();
    auto cloned_variable = static_unique_ptr_cast<Variable>(std::move(cloned_variable_term));
    auto cloned_body = this->body->clone();
    return std::make_unique<Abstraction>(std::move(cloned_variable), std::move(cloned_body));
}

unique_ptr<Term> Abstraction::beta_reduce() const {
    if (this->is_normal()) throw ReductionOnNormalForm();
    return make_unique<Abstraction>(
        static_unique_ptr_cast<Variable>(this->variable->clone()),
        this->body->beta_reduce()
    );
}

bool Abstraction::is_normal() const {
    return this->body->is_normal();
}

bool Abstraction::has_free(std::string target) const {
    if (this->variable->name == target) return false;
    return this->body->has_free(target);
}

std::string Abstraction::to_string() const {
    return "λ" + this->variable->name + ". " + this->body->to_string();
}