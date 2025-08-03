//
// Created by SeanLi on 8/3/25.
//

#include "Exceptions.h"

const char* ReductionOnNormalForm::what() const noexcept {
	return fullMessage.c_str();
}


const char* UndeclaredVariableError::what() const noexcept {
	return fullMessage.c_str();
}

const char* NotAFunctionError::what() const noexcept {
	return TypeMismatchError::what();
}

const char* TypeMismatchError::what() const noexcept {
	return std::runtime_error::what();
}

const char* DomainTypeMismatchError::what() const noexcept {
	return TypeMismatchError::what();
}