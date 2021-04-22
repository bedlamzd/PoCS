#include <iostream>
#include <vector>

#include "DiscreteStateSpace.h"

int main() {
    const double t0 = 0;
    const double tf = 50;
    const double stride = 0.01;
    const double dt = 0.2;
    std::vector<double> t_data;
    std::vector<double> u_data;
    std::vector<double> y_data;

    double t = t0;

    Matrix<3, 1> gen_X0 = Matrix<3, 1>(valarray<double_t>({1, 1, 10}));

    Matrix<3, 3> gen_A = Matrix<3, 3>(valarray<double_t>({
                                                                 .8187, 0, 0,
                                                                 0, .6703, 0,
                                                                 0, 0, 1
                                                         }));

    Matrix<1, 3> gen_C = Matrix<1, 3>(valarray<double_t>({-1, 3, 1}));

    auto generator = new DiscreteStateSpace<3, 1, 1>(dt);
    generator->set_A(gen_A);
    generator->set_C(gen_C);
    generator->set_X(gen_X0);


    Matrix<3, 3> obj_A = Matrix<3, 3>(valarray<double_t>({
                                                                 .9994, .1987, .0192,
                                                                 -.0096, .9802, .1872,
                                                                 -.0936, -.1968, .8679
                                                         }));

    Matrix<3, 1> obj_B = Matrix<3, 1>(valarray<double_t>({
                                                                 .0013,
                                                                 .0192,
                                                                 .1872
                                                         }));

    Matrix<1, 3> obj_C = Matrix<1, 3>(valarray<double_t>({.1, 10, 1}));

    auto object = new DiscreteStateSpace<3, 1, 1>(dt);
    object->set_A(obj_A);
    object->set_B(obj_B);
    object->set_C(obj_C);

    while (t < tf) {
        auto U = generator->compute(t);
        auto Y = object->compute(t, U);
        t_data.push_back(t);
        u_data.push_back(U(0, 0));
        y_data.push_back(Y(0, 0));
        t += stride;
    }

    for (int i = 0; i < y_data.size(); ++i) {
        std::cout << u_data[i] << " " << y_data[i] << std::endl;
    }

    return 0;
}
