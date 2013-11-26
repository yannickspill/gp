#ifndef DOUBLE_INPUT_CACHED_VERSION_TRACKER_H
#define DOUBLE_INPUT_CACHED_VERSION_TRACKER_H

//! Class that implements version tracking only, no caching
//Inputs need to have an `unsigned update()` call that returns their version
//number.
template <class In1Type, class In2Type>
class DoubleInputCachedVersionTracker {

    In1Type in1_;
    In2Type in2_;
    unsigned vin1_, vin2_, version_;
    mutable bool cache_invalid_;

   public:
    DoubleInputCachedVersionTracker(In1Type in1, In2Type in2)
        : in1_(in1),
          in2_(in2),
          vin1_(in1_.update()),
          vin2_(in2_.update()),
          version_(0),
          cache_invalid_(true) {}

    unsigned update() {
        std::cout << "doubleinpcached: update" << std::endl;
        unsigned vin1 = in1_.update();
        unsigned vin2 = in2_.update();
        if (vin1 != vin1_ || vin2 != vin2_){
            vin1_ = vin1;
            vin2_ = vin2;
            cache_invalid_ = true;
        std::cout << "   invalidated" << std::endl;
            version_++;
        }
        return version_;
    }

    bool cache_is_invalid() const { return cache_invalid_; }
    void set_cache_is_valid() const { cache_invalid_=false; }

};

#endif /* DOUBLE_INPUT_CACHED_VERSION_TRACKER_H */
