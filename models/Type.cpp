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
	return this->domain->to_string() + " -> " + this->codomain->to_string();
}

std::unique_ptr<Type> FunctionType::clone() const {
	return std::make_unique<FunctionType>(*this);
}