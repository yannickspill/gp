#ifndef SCALAR_H
#define SCALAR_H

#include <limits>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

// Scalar with optional upper/lower bounds
class Scalar {
   public:
    explicit Scalar(double value,
                    double lower = -std::numeric_limits<double>::infinity(),
                    double upper = std::numeric_limits<double>::infinity())
        : data_(boost::make_shared<Data>(value, lower, upper)) {}

    Scalar(const Scalar& other) : data_(other.data_) {}

    void clear_lower() {
        data_->lower_ = -std::numeric_limits<double>::infinity();
    }
    void clear_upper() {
        data_->upper_ = std::numeric_limits<double>::infinity();
    }

    void set_lower(double lower) {
        data_->lower_ = lower;
        if (get_upper() < lower) set_upper(lower);
        if (get() < lower) set(lower);
    }
    void set_upper(double upper) {
        data_->upper_ = upper;
        if (get_lower() > upper) set_lower(upper);
        if (get() > upper) set(upper);
    }

    double get_lower() const { return data_->lower_; }
    double get_upper() const { return data_->upper_; }

    double get() const { return data_->value_; }

    void set(double value) {
        double target = value;
        if (get_lower() > value) target = get_lower();
        if (get_upper() < value) target = get_upper();
        data_->value_ = target;
    }

   private:
    struct Data {
        double value_, lower_, upper_;
        Data(double v, double l, double u) : value_(v), lower_(l), upper_(u) {}
    };
    boost::shared_ptr<Data> data_;
};

#endif /* SCALAR_H */
