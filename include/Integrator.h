//
// Created by bedlamzd on 17.04.2021.
//

#ifndef POCS_INTEGRATOR_H
#define POCS_INTEGRATOR_H

class Integrator {
public:

    Integrator() : prev_input_(0), prev_time_(0), output_(0) {}

    explicit Integrator(double_t C) : prev_input_(0), prev_time_(0), output_(C) {}

    Integrator(double_t t0, double_t dx0, double_t C) : prev_input_(dx0), prev_time_(t0), output_(C) {}

    Integrator(Integrator &other) = default;

    explicit Integrator(Integrator *other) : prev_input_(other->prev_input_), prev_time_(other->prev_time_),
                                             output_(other->output_) {};

    Integrator &operator=(Integrator const &other) = default;

    Integrator &operator=(Integrator *other) {
        *this = Integrator(other);
        return *this;
    }

//    Integrator &operator=(Integrator &&other) noexcept {
//        this->prev_time_ = other.prev_time_;
//        this->prev_input_ = other.prev_input_;
//        this->output_ = other.output_;
//        return *this;
//    }

    double_t compute(double_t time, double_t input) {
        output_ += (input + prev_input_) / 2 * (time - prev_time_);
        prev_input_ = input;
        prev_time_ = time;
        return output_;
    }

    double_t get_output() {
        return output_;
    }

private:
    double prev_input_;
    double prev_time_;
    double output_;
};


#endif //POCS_INTEGRATOR_H
