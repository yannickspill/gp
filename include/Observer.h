#ifndef OBSERVER_H
#define OBSERVER_H

#include <set>

template <typename T> class Observer;

//Subject of an observer pattern
//stores a value and informs observers if it changed.
template <typename T> class Subject
{
    public:
        Subject(T value) : value_(value) {};

        T get_value() const { return value_; }

        //notify observers when value changes
        void set_value(T value) {
            value_ = value;
            typename std::set<Observer<T> *>::iterator it;
            for (it = observers_.begin(); it != observers_.end(); ++it)
                (*it)->notify(this);
        }

        //register observer. Returns false if already registered.
        bool register_observer(Observer<T>* o) {
            return observers_.insert(o).second;
        }

        //unregister observer. Returns false if already deleted.
        bool unregister_observer(Observer<T> *o) {
            return bool(observers_.erase(o));
        }

        virtual ~Subject() {};

    private:
        T value_;
        typename std::set<Observer<T> *> observers_;
};


template <typename T> class Observer
{
    public:
        virtual void notify(Subject<T> *s) = 0;
        virtual ~Observer() {};
};

#endif /* #OBSERVER_H */
