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

std::unique_ptr<Term> Application::beta_reduce() const {
	if (const auto* func_term = dynamic_cast<const Abstraction*>(this->function.get())) {
		return func_term->body->substitute(
			func_term->variable->name,
			*this->value
		);
	};
	if (!this->function->is_normal()) {
		return make_unique<Application>(this->function->beta_reduce(), this->value->clone());
	}
	if (!this->value->is_normal()) {
		return make_unique<Application>(this->function->clone(), this->value->beta_reduce());
	}
	throw ReductionOnNormalForm();
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