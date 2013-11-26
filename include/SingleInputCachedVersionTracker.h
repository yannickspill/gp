#ifndef SINGLE_INPUT_CACHED_VERSION_TRACKER_H
#define SINGLE_INPUT_CACHED_VERSION_TRACKER_H

#include <memory>

//! Class that implements version tracking and caching
//Input needs to have an `unsigned update()` call that returns its version
//number. Cached object is to be stored in the derived class.
template <class InType>
class SingleInputCachedVersionTracker {

    struct Data {
        InType in_;
        unsigned vin_, version_;
        mutable bool cache_invalid_;
        Data(InType in)
            : in_(in), vin_(in_.update()), version_(0), cache_invalid_(true) {}
    };
    std::shared_ptr<Data> data_;

   public:
    SingleInputCachedVersionTracker(InType in)
        : data_(std::make_shared<Data>(in)) {}

    unsigned update() {
        unsigned vin = data_->in_.update();
        if (vin != data_->vin_){
            data_->cache_invalid_ = true;
            data_->version_++;
        }
        data_->vin_ = vin;
        return data_->version_;
    }

    bool cache_is_invalid() const { return data_->cache_invalid_; }
    void set_cache_is_valid() const { data_->cache_invalid_=false; }

};

#endif /* SINGLE_INPUT_CACHED_VERSION_TRACKER_H */
