#include "Observer.h"

class IntGetter : public Observer<int>
{
    public:
    IntGetter() {val_=0;}

    virtual void notify(Subject<int> *s){
        val_ = s->get_value();
    }

    int get() { return val_; };

    private:
    int val_;
};

int main(int, char *[]) {
    IntGetter *obs = new IntGetter();
    Subject<int> *subj = new Subject<int>(1);
    if (obs->get() != 0) return 1;

    obs->notify(subj);
    if (obs->get() != 1) return 2;

    subj->register_observer(obs);
    subj->set_value(2);
    if (obs->get() != 2) return 3;

    subj->register_observer(obs);
    subj->set_value(3);
    if (obs->get() != 3) return 4;

    subj->unregister_observer(obs);
    subj->set_value(4);
    if (obs->get() != 3) return 5;

    return 0;
}
