//
// Created by SeanLi on 8/3/25.
//

#ifndef TYPE_H
#define TYPE_H

#include <memory>
#include <string>

#include "terms/Variable.h"


class Type {
public:
	virtual ~Type() = 0;
	[[nodiscard]] virtual std::string to_string() const = 0;
	[[nodiscard]] virtual std::unique_ptr<Type> clone() const = 0;
	// [[nodiscard]] virtual std::unique_ptr<Type> downcast() const = 0;
};

class BaseType : public Type {
public:
	std::string name;

	explicit BaseType(std::string name):
		name(std::move(name))
	{};

	[[nodiscard]] std::string to_string() const override;
	[[nodiscard]] std::unique_ptr<Type> clone() const override;
};

class FunctionType : public Type {
public:
	std::unique_ptr<Type> domain;
	std::unique_ptr<Type> codomain;

	explicit FunctionType(std::unique_ptr<Type> domain, std::unique_ptr<Type> codomain):
		domain(std::move(domain)),
		codomain(std::move(codomain))
	{}

	[[nodiscard]] std::string to_string() const override;
	[[nodiscard]] std::unique_ptr<Type> clone() const override;
};

#endif //TYPE_H
