//
// Created by SeanLi on 8/3/25.
//

#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <type_traits>

template<typename To, typename From>
std::unique_ptr<To> static_unique_ptr_cast(std::unique_ptr<From>&& p) {
	if (!p) {
		return nullptr;
	}
	// We explicitly use the default deleter for the destination type.
	return std::unique_ptr<To>(static_cast<To*>(p.release()));
}
#endif //UTILS_H
