//
// Created by zhenxiao on 3/7/19.
//

#ifndef XV6_PUBLIC_THREAD_H
#define XV6_PUBLIC_THREAD_H
#include <stdatomic.h>
int thread_create(void *(*routine)(void*), void *arg);

// Spin Lock
struct lock_t{
    uint flag;
};

//mcs
struct mcs_node
{
    int pid;
    int is_locked;
    struct mcs_node *next;
};
/*
struct mcs_lock
{
    struct msc_node *next;
    
};*/

void mcs_lock_init(struct mcs_node *node);
void mcs_lock_acquire(struct mcs_node *node, int pid);
void mcs_lock_release(struct mcs_node *node, int pid);


void lock_init(struct lock_t *lock);
void lock_acquire(struct lock_t *lock);
void lock_release(struct lock_t *lock);

// Anderson Lock (Array Lock)
#define ANDERSON_LOCK_MAXSIZE 100
struct anderson_lock_t {
    int next_ticket;
    uint can_serve[ANDERSON_LOCK_MAXSIZE];
};
void anderson_lock_init(struct anderson_lock_t *lock);
void anderson_lock_acquire(struct anderson_lock_t *lock, int *my_ticket);
void anderson_lock_release(struct anderson_lock_t *lock, int *my_ticket);

// SeqLock
struct seq_lock_t {
    int counter;
    uint flag;
};
void seq_lock_init(struct seq_lock_t *lock);
int seq_lock_get_counter(struct seq_lock_t *lock);
void seq_lock_acquire(struct seq_lock_t *lock);
void seq_lock_release(struct seq_lock_t *lock);


#endif //XV6_PUBLIC_THREAD_H
