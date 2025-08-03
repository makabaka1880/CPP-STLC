#include "Variable.h"

#include "../exceptions/Exceptions.h"
#include "Terms.h"


using std::unique_ptr; using std::make_unique;
Variable::Variable(std::string name): name(std::move(name)) {
}

Variable::~Variable() = default;

std::unique_ptr<Term> Variable::substitute(std::string target, Term& newValue) const {
    if (target == name) { return newValue.clone(); } else { return make_unique<Variable>(name); }
}

std::unique_ptr<Term> Variable::alpha_convert(const std::string newValue) const {
    return make_unique<Variable>(newValue);
}

std::unique_ptr<Term> Variable::beta_reduce() const {
    throw ReductionOnNormalForm();
}

std::unique_ptr<Term> Variable::clone() const {
    return std::make_unique<Variable>(name);
}

bool Variable::is_normal() const {
    return true;
}

bool Variable::has_free(std::string target) const {
    return this->name == target;
}
