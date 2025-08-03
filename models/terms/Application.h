//
// Created by SeanLi on 8/3/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include "../Terms.h"

#include <memory>


class Application : public Term {
public:
	std::unique_ptr<Term> function;
	std::unique_ptr<Term> value;
	explicit Application(std::unique_ptr<Term> function, std::unique_ptr<Term> value):
		function(std::move(function)),
		value(std::move(value))
	{};
	~Application() override;

	[[nodiscard]] std::unique_ptr<Term> alpha_convert(std::string newValue) const override;
	[[nodiscard]] std::unique_ptr<Term> substitute(std::string target, Term& newValue) const override;
	[[nodiscard]] std::unique_ptr<Term> clone() const override;
	[[nodiscard]] std::unique_ptr<Term> beta_reduce() const override;
	[[nodiscard]] std::unique_ptr<Type> type_check(const TypingContext& context) const override;
	[[nodiscard]] bool is_normal() const override;
    [[nodiscard]] bool has_free(std::string target) const override;
	[[nodiscard]] std::string to_string() const override;
};



#endif //APPLICATION_H
