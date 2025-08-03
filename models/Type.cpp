//
// Created by SeanLi on 8/3/25.
//

#include "Type.h"

#include "terms/Variable.h"

Type::~Type() = default;

BaseType::~BaseType() = default;

std::string BaseType::to_string() const {
	return this->name;
}

std::unique_ptr<Type> BaseType::clone() const {
	return std::make_unique<BaseType>(*this);
}

FunctionType::~FunctionType() = default;

FunctionType::FunctionType(const FunctionType& other)
	: domain(other.domain->clone()),
	  codomain(other.codomain->clone())
{}

std::string FunctionType::to_string() const {
	std::string domain_str = this->domain->to_string();
	std::string codomain_str = this->codomain->to_string();

	// Check if the domain is a FunctionType using dynamic_cast
	if (const auto func_type_domain = dynamic_cast<const FunctionType*>(this->domain.get())) {
		domain_str = "(" + domain_str + ")";
	}

	// Check if the codomain is a FunctionType using dynamic_cast
	if (const auto func_type_domain = dynamic_cast<const FunctionType*>(this->codomain.get())) {
		codomain_str = "(" + codomain_str + ")";
	}

	return domain_str + " -> " + codomain_str;
}

std::unique_ptr<Type> FunctionType::clone() const {
	return std::make_unique<FunctionType>(*this);
}