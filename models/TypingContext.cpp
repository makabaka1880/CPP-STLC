//
// Created by SeanLi on 8/3/25.
//
#include "TypingContext.h"
#include <map>

TypingContext::TypingContext() = default;
TypingContext::~TypingContext() = default;

const Type* TypingContext::lookup(const std::string& name) const {
	auto it = context.find(name);
	if (it != context.end()) {
		return it->second;
	}
	return nullptr;
}