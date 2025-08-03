//
// Created by SeanLi on 8/3/25.
//

#include "Terms.h"

Term::~Term() = default;

std::ostream& operator<<(std::ostream& os, const Term& term) {
	os << term.to_string();
	return os;
}