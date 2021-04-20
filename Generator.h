//
// Created by bedlamzd on 17.04.2021.
//

#ifndef POCS_GENERATOR_H
#define POCS_GENERATOR_H

#include "Integrator.h"

class Generator {
public:

    explicit Generator(double *time, double *output) :t_(time), output_(output) {};

    void compute() {
        dx1 = -x1;
        dx2 = -2*x2;

        integrator_1->compute();
        integrator_2->compute();

        *output_ = -x1 + 3 * x2 + x3;
    }


private:

    double *t_;

    double x1 = 1;
    double dx1;
    double x2 = 1;
    double dx2;
    double x3 = 10;

    double *output_;

    Integrator *integrator_1 = new Integrator(t_, &dx1, &x1, x2);
    Integrator *integrator_2 = new Integrator(t_, &dx2, &x2, x2);

};

#endif //POCS_GENERATOR_H
