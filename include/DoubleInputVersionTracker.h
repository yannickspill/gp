#ifndef DOUBLE_INPUT_VERSION_TRACKER_H
#define DOUBLE_INPUT_VERSION_TRACKER_H

#include "macros.h"
#include "Scalar.h"
#include <memory>

//! Class that implements version tracking only, no caching
//Inputs need to have an `unsigned update()` call that returns their version
//number.
template <class In1Type, class In2Type>
class DoubleInputVersionTracker {

    struct Data {
        In1Type in1_;
        In2Type in2_;
        unsigned vin1_, vin2_, version_;
        Data(In1Type in1, In2Type in2)
            : in1_(in1),
              in2_(in2),
              vin1_(in1_.update()),
              vin2_(in2_.update()),
              version_(0) {}
    };
    std::shared_ptr<Data> data_;

  public:

    static const unsigned num_inputs = 2;

    DoubleInputVersionTracker(In1Type in1, In2Type in2)
        : data_(std::make_shared<Data>(in1, in2)) {}

    unsigned update() {
        LOG("DoubleInputVersionTracker::update()");
        unsigned vin1 = data_->in1_.update();
        unsigned vin2 = data_->in2_.update();
        if (vin1 != data_->vin1_ || vin2 != data_->vin2_) {
            data_->vin1_ = vin1;
            data_->vin2_ = vin2;
            data_->version_++;
            LOG(" increment version");
        }
        LOG(std::endl);
        return data_->version_;
    }

    bool get_is_function_of(const Scalar& s) const {
        return data_->in1_.get_is_function_of(s) ||
               data_->in2_.get_is_function_of(s);
    }
};

#endif /* DOUBLE_INPUT_VERSION_TRACKER_H */
