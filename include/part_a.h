/* part_a.h */

#ifndef PART_A_H
#define PART_A_H

#include "Matrix.h"

template <typename T> Matrix<T> getMean(Matrix<T> const&);
template<typename T> Matrix<T> getRollMean(Matrix<T> const&, std::size_t, std::size_t);
template <typename T> Matrix<T> getCovMatrix(Matrix<T> const&, std::size_t, std::size_t);

#include "part_a.cpp"

#endif