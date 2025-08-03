//
// Created by SeanLi on 8/3/25.
//

#include "Type.h"

Type::~Type() = default;

std::string BaseType::to_string() const {
	return this->name;
}

std::string FunctionType::to_string() const {
	return this->domain->to_string() + " -> " + this->codomain->to_string();
}