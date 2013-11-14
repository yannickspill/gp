#ifndef SCALAR_H
#define SCALAR_H

#include "Observer.h"

//Scalar with optional upper/lower bounds
class Scalar : public Subject<double>
{
    public:
        Scalar(double value) : Subject(value),
            has_upper_(false), has_lower_(false) {};

        bool has_lower() const { return has_lower_; }
        bool has_upper() const { return has_upper_; }

        void clear_lower() { has_lower_ = false; }
        void clear_upper() { has_upper_ = false; }

        void set_lower(double lower) {
            lower_ = lower;
            if (has_upper() and (get_upper() < lower_)) set_upper(lower);
            has_lower_ = true;
            if (get() < lower_) set(lower_);
        }
        void set_upper(double upper) {
            upper_ = upper;
            if (has_lower() and (get_lower() > upper_)) set_lower(upper);
            has_upper_ = true;
            if (get() > upper_) set(upper_);
        }

        double get_lower() {return lower_;}
        double get_upper() {return upper_;}

    protected:
        virtual void setval(double value) {
            double target = value;
            if (has_lower() and (get_lower() > value))
                target = get_lower();
            if (has_upper() and (get_upper() < value))
                target = get_upper();
            value_ = value;
        }

    private:
        bool has_upper_, has_lower_;
        double upper_, lower_;
};


#endif /* SCALAR_H */
