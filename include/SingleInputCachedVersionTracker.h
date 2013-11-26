#ifndef SINGLE_INPUT_CACHED_VERSION_TRACKER_H
#define SINGLE_INPUT_CACHED_VERSION_TRACKER_H

//! Class that implements version tracking and caching
//Input needs to have an `unsigned update()` call that returns its version
//number. Cached object is to be stored in the derived class.
template <class InType>
class SingleInputCachedVersionTracker {

    InType in_;
    unsigned vin_, version_;
    mutable bool cache_invalid_;

   public:
    SingleInputCachedVersionTracker(InType in)
        : in_(in),
          vin_(in_.update()),
          version_(0),
          cache_invalid_(true) {}

    unsigned update() {
        unsigned vin = in_.update();
        if (vin != vin_){
            cache_invalid_ = true;
            version_++;
        }
        vin_ = vin;
        return version_;
    }

    bool cache_is_invalid() const { return cache_invalid_; }
    void set_cache_is_valid() const { cache_invalid_=false; }

};

#endif /* SINGLE_INPUT_CACHED_VERSION_TRACKER_H */
