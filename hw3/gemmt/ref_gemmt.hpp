#ifndef REF_GEMMT_HPP
#define REFGEMMT_HPP

#include <vector>
#include <stdexcept>

template <typename T>
void gemm(T a, const std::vector<std::vector<T>> &A, const std::vector<std::vector<T>>,
            T b, std::vector<std::vector<T>> &C);

#endif