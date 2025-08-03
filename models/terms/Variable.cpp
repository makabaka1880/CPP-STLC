#include "Variable.h"

#include "../../exceptions/Exceptions.h"
#include "../Terms.h"


using std::unique_ptr; using std::make_unique;
Variable::Variable(std::string name): name(std::move(name)) {
}

Variable::~Variable() = default;

unique_ptr<Term> Variable::substitute(std::string target, Term& newValue) const {
    if (target == name) { return newValue.clone(); } else { return make_unique<Variable>(name); }
}

unique_ptr<Term> Variable::alpha_convert(const std::string newValue) const {
    return make_unique<Variable>(newValue);
}

unique_ptr<Term> Variable::beta_reduce() const {
    throw ReductionOnNormalForm(this->clone());
}

unique_ptr<Term> Variable::clone() const {
    return make_unique<Variable>(name);
}

unique_ptr<Type> Variable::type_check(const TypingContext& context) const {
    auto type = context.lookup(this->name);
    if (type == nullptr) { throw UndeclaredVariableError(this->name); }
    return type->clone();
}

bool Variable::is_normal() const {
    return true;
}

bool Variable::has_free(const std::string target) const {
    return this->name == target;
}

std::string Variable::to_string() const {
    return name;
}