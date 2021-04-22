#include <iostream>
#include <vector>

#include "StateSpace.h"

int main() {
    const double t0 = 0;
    const double tf = 10;
    const double stride = 0.01;
    std::vector<double> t_data;
    std::vector<double> y_data;

    double t = t0;

    Matrix<3, 1> X0 = Matrix<3, 1>(valarray<double_t>({1, 1, 10}));

    Matrix<3, 3> A = Matrix<3, 3>(valarray<double_t>({-1, 0, 0,
                                                              0, -2, 0,
                                                              0, 0, 0}));

    Matrix<1, 3> C = Matrix<1, 3>(valarray<double_t>({-1, 3, 1}));

    auto ss = new StateSpace<3, 1, 1>();
    ss->set_A(A);
    ss->set_C(C);
    ss->set_X(X0);

    while (t < tf) {
        auto Y = ss->compute(t);
        t_data.push_back(t);
        y_data.push_back(Y(0, 0));
        t += stride;
    }


    for (auto &&y:y_data) {
        std::cout << y << std::endl;
    }

    return 0;
}
