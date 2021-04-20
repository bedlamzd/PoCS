#include <iostream>
#include <vector>

#include "Generator.h"

int main() {
    const double t0 = 0;
    const double tf = 10;
    const double stride = 0.01;
    const int N = (int) ((tf - t0) / stride);
    std::vector<double> t_data;
    std::vector<double> y_data;

    double t = t0;
    double output;
    auto *generator = new Generator(&t, &output, stride);

    for (int i = 0; i < N; ++i) {
        generator->compute();
        t_data.push_back(t);
        y_data.push_back(output);
        t += stride;
    }

    for (auto &&y:y_data) {
        std::cout << y << std::endl;
    }

    return 0;
}
