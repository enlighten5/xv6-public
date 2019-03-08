//
// Created by zhenxiao on 3/7/19.
//

#ifndef XV6_PUBLIC_THREAD_H
#define XV6_PUBLIC_THREAD_H
struct lock_t{
    uint flag;
};

int thread_create(void *(*routine)(void*), void *arg);
void lock_init(struct lock_t *lock);
void lock_acquire(struct lock_t *lock);
void lock_release(struct lock_t *lock);
#endif //XV6_PUBLIC_THREAD_H
