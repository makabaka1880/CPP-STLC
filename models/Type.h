//
// Created by SeanLi on 8/3/25.
//

#ifndef TYPE_H
#define TYPE_H

#include <memory>
#include <string>


class Type {
public:
	virtual ~Type() = 0;
};

class BaseType : public Type {
public:
	std::string name;
	explicit BaseType(std::string name) : name(std::move(name)) {}
};

class FunctionType : public Type {
public:
	std::unique_ptr<Type> domain;
	std::unique_ptr<Type> codomain;
	explicit FunctionType(std::unique_ptr<Type> domain, std::unique_ptr<Type> codomain):
		domain(std::move(domain)),
		codomain(std::move(codomain))
	{}
};

#endif //TYPE_H
