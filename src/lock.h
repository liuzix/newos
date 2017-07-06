//
// Created by Zixiong Liu on 6/7/17.
//

#ifndef MYOS_LOCK_H
#define MYOS_LOCK_H

namespace synchronization {
    class SpinLock {
        bool lockvar;
    public:
        SpinLock ();
        void lock ();
        void unlock ();
        bool trylock ();
    };
}

#endif //MYOS_LOCK_H
