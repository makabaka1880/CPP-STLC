//
// Created by SeanLi on 8/3/25.
//

#include "Type.h"

#include "terms/Variable.h"

Type::~Type() = default;

std::string BaseType::to_string() const {
	return this->name;
}

std::unique_ptr<Type> BaseType::clone() const {
	return std::make_unique<Type>(static_cast<const Type&>(*this));
}

std::string FunctionType::to_string() const {
	return this->domain->to_string() + " -> " + this->codomain->to_string();
}

std::unique_ptr<Type> FunctionType::clone() const {
	return std::make_unique<Type>(static_cast<const Type&>(*this));
}