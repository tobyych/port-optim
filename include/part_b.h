/* part_b.h */

#ifndef PART_B_H
#define PART_B_H

#include "Matrix.h"

template <typename T> Matrix<T> getQMatrix(Matrix<T> const&, Matrix<T> const&, Matrix<T> const&);
template <typename T> Matrix<T> conjugateGradient(Matrix<T>&, Matrix<T> const&, Matrix<T> const&, T, std::size_t);

#include "part_b.cpp"

#endif