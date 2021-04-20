//
// Created by bedlamzd on 17.04.2021.
//

#ifndef POCS_INTEGRATOR_H
#define POCS_INTEGRATOR_H

#include <cmath>

class Integrator {
public:

    Integrator(double *time, double *input, double *output, double C) :
            time_(time), input_(input), output_(output), initial_cond_(C) {
        prev_time_ = *time_;
        prev_input = *input_;
        *output_ = initial_cond_;
    }

    void compute() {
        *output_ += (*input_ + prev_input) / 2 * (*time_ - prev_time_);
        prev_time_ = *time_;
        prev_input = *input_;
    }

private:
    double *input_;
    double *output_;
    double *time_;
    const double initial_cond_;
    double prev_input;
    double prev_time_;
};


#endif //POCS_INTEGRATOR_H
