//
// Created by SeanLi on 8/3/25.
//
#include "TypingContext.h"
#include <map>

TypingContext::TypingContext() = default;
TypingContext::~TypingContext() = default;

const Type* TypingContext::lookup(const std::string& name) const {
	if (const auto it = context.find(name); it != context.end()) {
		return it->second;
	}
	return nullptr;
}

void TypingContext::add(const std::string& name, const Type* type) {
	this->context[name] = type;
}