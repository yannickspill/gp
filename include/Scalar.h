#ifndef SCALAR_H
#define SCALAR_H

#include <limits>
#include <memory>

// Scalar with optional upper/lower bounds
class Scalar {
   public:
    explicit Scalar(double value,
                    double lower = -std::numeric_limits<double>::infinity(),
                    double upper = std::numeric_limits<double>::infinity())
        : data_(std::make_shared<Data>(value, lower, upper)) {}

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

    typedef double return_type;
    return_type get() const { return data_->value_; }

    void set(double value) {
        double target = value;
        if (get_lower() > value) target = get_lower();
        if (get_upper() < value) target = get_upper();
        data_->value_ = target;
        (data_->version_)++;
    }

    unsigned update() const {
        //Scalar is updated when set() is called
        return data_->version_;
    }

    bool operator==(const Scalar& other) const {
        return data_ == other.data_;
    }
    bool operator!=(const Scalar& other) const {
        return !(*this == other);
    }

   private:
    struct Data {
        double value_, lower_, upper_;
        unsigned version_;
        Data(double v, double l, double u)
            : value_(v), lower_(l), upper_(u), version_(0) {}
    };
    std::shared_ptr<Data> data_;
};

#endif /* SCALAR_H */
