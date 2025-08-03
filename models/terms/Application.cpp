//
// Created by SeanLi on 8/3/25.
//

#include "Abstraction.h"

#include "Application.h"

#include "../../exceptions/Exceptions.h"

using std::unique_ptr, std::make_unique;

Application::~Application() = default;

unique_ptr<Term> Application::clone() const {
	return make_unique<Application>(
		  this->function->clone(),
		  this->value->clone()
	   );
}

unique_ptr<Term> Application::alpha_convert(std::string newValue) const {
	return make_unique<Application>(
		this->function->alpha_convert(newValue),
		this->value->alpha_convert(newValue)
	);
}

unique_ptr<Term> Application::substitute(std::string target, Term& newValue) const {
	return make_unique<Application>(
		this->function->substitute(target, newValue),
		this->value->substitute(target, newValue)
	);
}

unique_ptr<Term> Application::beta_reduce() const {
	if (const auto* func_term = dynamic_cast<const Abstraction*>(this->function.get())) {
		return func_term->body->substitute(
			func_term->var_name,
			*this->value
		);
	};
	if (!this->function->is_normal()) {
		return make_unique<Application>(this->function->beta_reduce(), this->value->clone());
	}
	if (!this->value->is_normal()) {
		return make_unique<Application>(this->function->clone(), this->value->beta_reduce());
	}
	throw ReductionOnNormalForm(this->value);
}

std::unique_ptr<Type> Application::type_check(const TypingContext &context) const {
	auto func_type_result = this->function->type_check(context);

	if (func_type_result == nullptr) {
		throw NotAFunctionError("The function has no type");
	}

	if (const auto func_is_base_type = dynamic_cast<const BaseType*>(func_type_result.get())) {
		throw NotAFunctionError(func_is_base_type->name);
	} else if (const auto func_is_func_type = dynamic_cast<const FunctionType*>(func_type_result.get())) {
		auto value_type = this->value->type_check(context);

		if (value_type == nullptr) {
		} else if (value_type->to_string() != func_is_func_type->domain->to_string()) {
			throw DomainTypeMismatchError(value_type->to_string(), func_is_func_type->domain->to_string());
		}

		return func_is_func_type->codomain->clone();
	} else {
		throw NotAFunctionError(func_type_result->to_string());
	}
}


bool Application::is_normal() const {
	if (const auto* func_term = dynamic_cast<const Abstraction*>(this->function.get())) return false;
	else {
		return this->function->is_normal() && this->value->is_normal();
	};
}

bool Application::has_free(std::string target) const {
	return this->function->has_free(target) || this->value->has_free(target);
}

std::string Application::to_string() const {
	return "(" + this->function->to_string() + ") (" + this->value->to_string() + ")";
}