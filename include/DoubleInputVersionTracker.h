#ifndef DOUBLE_INPUT_VERSION_TRACKER_H
#define DOUBLE_INPUT_VERSION_TRACKER_H

//! Class that implements version tracking only, no caching
//Inputs need to have an `unsigned update()` call that returns their version
//number.
template <class In1Type, class In2Type>
class DoubleInputVersionTracker {

    In1Type in1_;
    In2Type in2_;
    unsigned vin1_, vin2_, version_;

   public:
    DoubleInputVersionTracker(In1Type in1, In2Type in2)
        : in1_(in1),
          in2_(in2),
          vin1_(in1_.update()),
          vin2_(in2_.update()),
          version_(0) {}

    unsigned update() {
        unsigned vin1 = in1_.update();
        unsigned vin2 = in2_.update();
        if (vin1 != vin1_ || vin2 != vin2_){
            vin1_ = vin1;
            vin2_ = vin2;
            version_++;
        }
        return version_;
    }

};

#endif /* DOUBLE_INPUT_VERSION_TRACKER_H */
