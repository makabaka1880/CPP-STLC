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

const char* TypeMismatchError::what() const noexcept {
	return fullMessage.c_str();
}