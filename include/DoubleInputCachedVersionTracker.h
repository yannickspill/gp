#ifndef DOUBLE_INPUT_CACHED_VERSION_TRACKER_H
#define DOUBLE_INPUT_CACHED_VERSION_TRACKER_H

#include <memory>

//! Class that implements version tracking only, no caching
//Inputs need to have an `unsigned update()` call that returns their version
//number.
template <class In1Type, class In2Type>
class DoubleInputCachedVersionTracker {

    struct Data {
        In1Type in1_;
        In2Type in2_;
        unsigned vin1_, vin2_, version_;
        mutable bool cache_invalid_;
        Data(In1Type in1, In2Type in2)
            : in1_(in1),
              in2_(in2),
              vin1_(in1_.update()),
              vin2_(in2_.update()),
              version_(0),
              cache_invalid_(true) {}
    };
    std::shared_ptr<Data> data_;

   public:
    DoubleInputCachedVersionTracker(In1Type in1, In2Type in2)
        : data_(std::make_shared<Data>(in1,in2)) {}

    unsigned update() {
        unsigned vin1 = data_->in1_.update();
        unsigned vin2 = data_->in2_.update();
        if (vin1 != data_->vin1_ || vin2 != data_->vin2_){
            data_->vin1_ = vin1;
            data_->vin2_ = vin2;
            data_->cache_invalid_ = true;
            data_->version_++;
        }
        return data_->version_;
    }

    bool cache_is_invalid() const { return data_->cache_invalid_; }
    void set_cache_is_valid() const { data_->cache_invalid_=false; }

};

#endif /* DOUBLE_INPUT_CACHED_VERSION_TRACKER_H */
