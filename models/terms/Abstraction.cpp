//
// Created by SeanLi on 8/3/25.
//

#include "Abstraction.h"

#include "../Terms.h"
#include "../../utils.h"
#include "../../exceptions/Exceptions.h"

using std::unique_ptr; using std::make_unique;

Abstraction::Abstraction(std::unique_ptr<Variable> variable, std::unique_ptr<Term> body):
    var_type(variable->get_type().clone()),
    var_name(variable->name),
    body(std::move(body))
{};

Abstraction::~Abstraction() = default;

unique_ptr<Term> Abstraction::alpha_convert(const std::string newValue) const {
    auto new_variable = make_unique<Variable>(newValue, this->var_type->clone());
    auto new_body = this->body->substitute(this->var_name, *new_variable);
    return make_unique<Abstraction>(
        this->var_type->clone(),
        newValue,
        std::move(new_body)
    );
}

unique_ptr<Term> Abstraction::substitute(std::string target, Term& newValue) const {
    // Target var is same as binding var, stop
    if (this->var_name == target) {
        return this->clone();
    }

    // Free var capturing
    if (newValue.has_free(this->var_name)) {
        auto new_var = make_unique<Variable>(this->var_name+"'", this->var_type->clone());
        return new_var->substitute(target, newValue);
    }

    // Everything's nice
    return make_unique<Abstraction>(
        this->var_type->clone(),
        this->var_name,
        this->body->substitute(target, newValue)
    );
}

unique_ptr<Term> Abstraction::clone() const {
    return std::make_unique<Abstraction>(this->var_type->clone(), this->var_name, this->body->clone());
}

unique_ptr<Term> Abstraction::beta_reduce() const {
    if (this->is_normal()) throw ReductionOnNormalForm(this->clone());
    return make_unique<Abstraction>(
        this->var_type->clone(),
        this->var_name,
        this->body->beta_reduce()
    );
}

std::unique_ptr<Type> Abstraction::type_check(const TypingContext& context) const {
    auto extended_ctx = context;
    extended_ctx.add(this->var_name, this->var_type.get());
    auto func_body_type = this->body->type_check(extended_ctx);
    return make_unique<FunctionType>(this->var_type->clone(), std::move(func_body_type));
}

bool Abstraction::is_normal() const {
    return this->body->is_normal();
}

bool Abstraction::has_free(std::string target) const {
    if (this->var_name == target) return false;
    return this->body->has_free(target);
}

std::string Abstraction::to_string() const {
    return "λ" + this->var_name + ". " + this->body->to_string();
}