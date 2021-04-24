//
// Created by bedlamzd on 21.04.2021.
//

#ifndef POCS_MATRIX_H
#define POCS_MATRIX_H

#include <valarray>
#include <initializer_list>

using std::valarray;
using std::slice;

template<unsigned _row, unsigned _col, class T=double_t>
class Matrix {
public:

    static const unsigned rows = _row;
    static const unsigned cols = _col;

    valarray<T> data = valarray<T>(_row * _col);

    Matrix() {
        this->data = 0;
    }

    explicit Matrix(valarray<T> arr) {
        data = arr;
    }

    Matrix(const Matrix<_row, _col, T> &other) {
        data = other.data;
    }

    T &operator()(int r, int c) { return data[r * cols + c]; }

    template<unsigned _o_col>
    friend Matrix<_row, _o_col, T> operator*(const Matrix<_row, _col, T> &A, const Matrix<_col, _o_col, T> &other) {

        Matrix<_row, _o_col, T> C;

        for (int r = 0; r < C.rows; ++r) {
            auto sl1 = slice(r * A.cols, A.cols, 1); // row of the A matrix
            for (int c = 0; c < C.cols; ++c) {
                auto sl2 = slice(c * other.rows, other.rows, other.cols); // column of the other matrix

                // sum of the elementwise product of row and column
                C(r, c) = valarray<T>(A.data[sl1] * other.data[sl2]).sum();
            }
        }
        return C;
    }

    Matrix<_row, _col, T> &operator+(const Matrix<_row, _col, T> &other) {

        this->data += other.data;
        return *this;
    }

};


#endif //POCS_MATRIX_H
