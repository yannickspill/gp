#include "Observer.h"

#include <boost/scoped_ptr.hpp>

class IntGetter : public Observer<int>
{
    public:
    IntGetter() {val_=0;}

    virtual void notify(Subject<int> *s){
        val_ = s->get();
    }

    int get() { return val_; };

    private:
    int val_;
};

class IntSetter : public Subject<int>
{
    public:
        IntSetter(int val) : Subject(val) {};

    protected:
        virtual void setval(int value) {
            value_ = value;
        }
};

int main(int, char *[]) {
    boost::scoped_ptr<IntGetter> obs(new IntGetter());
    boost::scoped_ptr<IntSetter> subj(new IntSetter(1));
    //NEWPTR(IntGetter, obs, () );
    //NEWPTR(IntSetter, subj, (1) );

    if (obs->get() != 0) return 1;

    obs->notify(subj.get());
    if (obs->get() != 1) return 2;

    subj->register_observer(obs.get());
    subj->set(2);
    if (obs->get() != 2) return 3;

    subj->register_observer(obs.get());
    subj->set(3);
    if (obs->get() != 3) return 4;

    subj->unregister_observer(obs.get());
    subj->set(4);
    if (obs->get() != 3) return 5;

    return 0;
}
