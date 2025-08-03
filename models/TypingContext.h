//
// Created by SeanLi on 8/3/25.
//

#ifndef TYPINGCONTEXT_H
#define TYPINGCONTEXT_H
#include <map>
#include "Type.h"


class TypingContext {
private:
	std::map<std::string, const Type*> context;
public:
	[[nodiscard]] std::unique_ptr<Type> get(std::string target) const;
	[[nodiscard]] const Type* lookup(const std::string& name) const;
};

#endif //TYPINGCONTEXT_H
