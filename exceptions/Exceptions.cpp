//
// Created by SeanLi on 8/3/25.
//

#include "Exceptions.h"

const char* ReductionOnNormalForm::what() const noexcept {
	return "Beta reduction on normal form";
}