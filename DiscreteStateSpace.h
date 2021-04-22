//
// Created by bedlamzd on 22.04.2021.
//

#ifndef POCS_DISCRETESTATESPACE_H
#define POCS_DISCRETESTATESPACE_H

#include <valarray>
#include <vector>

using std::vector;
using std::valarray;
using std::slice;

#include "Integrator.h"
#include "Matrix.h"

template<unsigned _n, unsigned _q, unsigned _p>
class DiscreteStateSpace {
public:
    DiscreteStateSpace() : sample_time_(0) {};

    explicit DiscreteStateSpace(double_t sample_time) : sample_time_(sample_time) {};

    void set_A(Matrix<_n, _n> A) {
        A_ = A;
    }

    void set_B(Matrix<_n, _p> B) {
        B_ = B;
    }

    void set_C(Matrix<_q, _n> C) {
        C_ = C;
    }

    void set_D(Matrix<_q, _p> D) {
        D_ = D;
    }


    void set_X(Matrix<_n, 1> X) {
        for (int i = 0; i < _n; ++i) {
            X_(i, 0) = X(i, 0);
        }
    }

    Matrix<_p, 1> compute(double_t t, Matrix<_p, 1> u) {
        Y_ = C_ * X_ + D_ * u;
        if (t - prev_t_ > sample_time_){
            X_ = A_ * X_ + B_ * u;
            prev_t_ = t;
        }
        return Y_;
    };

    Matrix<_p, 1> compute(double_t t) {
        return this->compute(t, Matrix<_p, 1>());
    };

    Matrix<_p, 1> get_output() {
        return Y_;
    };


private:
    Matrix<_n, 1> X_;
    Matrix<_p, 1> Y_;

    Matrix<_n, _n> A_;
    Matrix<_n, _p> B_;
    Matrix<_q, _n> C_;
    Matrix<_q, _p> D_;

    double_t prev_t_ = 0;
    const double_t sample_time_;
};

#endif //POCS_DISCRETESTATESPACE_H
