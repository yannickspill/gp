#ifndef OBSERVER_H
#define OBSERVER_H

#include <set>

template <typename T>
class Observer;

// Subject of an observer pattern
// stores a value and informs observers if it changed.
// must be subclassed and define the protected method
// virtual void setval(T value)
template <typename T>
class Subject {
   public:
    Subject(T value) : value_(value) {};

    T get() const { return value_; }

    void set(T value) {
        // set value, operation happens in child
        setval(value);
        // notify observers when value changes
        typename std::set<Observer<T>& >::iterator it;
        for (it = observers_.begin(); it != observers_.end(); ++it)
            (*it)->notify(this);
    }

    // register observer. Returns false if already registered.
    bool register_observer(Observer<T>& o) {
        return observers_.insert(o).second;
    }

    // unregister observer. Returns false if already deleted.
    bool unregister_observer(Observer<T>* o) {
        return bool(observers_.erase(o));
    }

    virtual ~Subject() {};

   protected:
    // these two are used in the child.
    virtual void setval(T value) = 0;
    T value_;

   private:
    typename std::set<Observer<T>*> observers_;
};

// Observer of an observer pattern
// has a notify() call that takes a subject as parameter
// must be subclassed and define the public method
// virtual void notify(Subject<T>* s)
template <typename T>
class Observer {
   public:
    virtual void notify(Subject<T>& s) = 0;
    virtual ~Observer() {};
};

#endif /* #OBSERVER_H */
