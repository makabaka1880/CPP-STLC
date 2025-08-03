//
// Created by SeanLi on 8/3/25.
//

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>

class ReductionOnNormalForm: public std::exception {
    [[nodiscard]] const char* what() const noexcept override;
};



#endif //EXCEPTIONS_H
