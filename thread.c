//
// Created by zhenxiao on 3/7/19.
//
#include "types.h"
//#include "defs.h"
#include "user.h"
#include "memlayout.h"
#include "mmu.h"
#include "param.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "thread.h"
#include "stddef.h"


int
thread_create(void *(*routine)(void*), void *arg){
    void *stack = malloc(PGSIZE*2);//page align
    int i = clone(stack, PGSIZE*2);
    if(i == 0){
        (*routine)(arg);
        free(stack);
        exit();
    } else
        return i;
}

void
lock_init(struct lock_t *lock){
    xchg(&lock->flag, 0);
}

void lock_acquire(struct lock_t *lock){
    while(xchg(&lock->flag, 1) == 1);
}

void lock_release(struct lock_t *lock){
    xchg(&lock->flag, 0);
}

// Anderson Lock
void
anderson_lock_init(struct anderson_lock_t *lock) {
    lock->next_ticket = 0;
    int i;
    for (i = 1; i < ANDERSON_LOCK_MAXSIZE; i++) {
        lock->can_serve[i] = 0;
    }
    lock->can_serve[0] = 1;
}

void
anderson_lock_acquire(struct anderson_lock_t *lock, int *my_ticket) {
    *my_ticket = fetch_and_add(&(lock->next_ticket), 1);
    // printf(1, "old_ticket=%d, new_ticket=%d\n", *my_ticket, lock->next_ticket);
    while (xchg(&lock->can_serve[(*my_ticket) % ANDERSON_LOCK_MAXSIZE], 0) != 1);
        // printf(1, "%d", lock->can_serve[(*my_ticket) % ANDERSON_LOCK_MAXSIZE]);
        // sleep(1);
    // printf(1, "lock acquired by ticket %d\n", *my_ticket);
}

void 
anderson_lock_release(struct anderson_lock_t *lock, int *my_ticket) {
    // printf(1, "before release: %d %d %d %d %d %d\n", lock->can_serve[0], lock->can_serve[1]
    // , lock->can_serve[2], lock->can_serve[3], lock->can_serve[4], lock->can_serve[5]);
    // lock->can_serve[*my_ticket % ANDERSON_LOCK_MAXSIZE] = 0;
    // xchg(&lock->can_serve[*my_ticket % ANDERSON_LOCK_MAXSIZE], 0);
    // lock->can_serve[(*my_ticket + 1) % ANDERSON_LOCK_MAXSIZE] = 1;
    xchg(&lock->can_serve[(*my_ticket + 1) % ANDERSON_LOCK_MAXSIZE], 1);
    // printf(1, "after release: %d %d %d %d %d %d\n", lock->can_serve[0], lock->can_serve[1]
    // , lock->can_serve[2], lock->can_serve[3], lock->can_serve[4], lock->can_serve[5]);
}

// SeqLock
void 
seq_lock_init(struct seq_lock_t *lock) {
    lock->counter = 0;
    lock->flag = 0;
}

int 
seq_lock_get_counter(struct seq_lock_t *lock) {
    return lock->counter;
}

void 
seq_lock_acquire(struct seq_lock_t *lock) {
    while(xchg(&lock->flag, 1) == 1);
    lock->counter++;
}

void 
seq_lock_release(struct seq_lock_t *lock) {
    lock->counter++;
    xchg(&lock->flag, 0);
}

//mcs
void mcs_lock_init(struct mcs_node *node) {
    node->next = NULL;
}
void mcs_lock_acquire(struct mcs_node *node, int pid) {
    struct mcs_node *new_node = (struct mcs_node *)malloc(sizeof(struct mcs_node));
    new_node->pid = pid;
    new_node->next = NULL;
    new_node->is_locked = 0;
    struct mcs_node *tail = node;
    if (node->next == NULL){
        node->next = new_node;
        return;
    }else
    {
        while(tail->next != NULL){
            tail = tail->next;
        }
        new_node->is_locked = 1;
        //fetch_and_store
        tail->next = new_node;
        while(new_node->is_locked){
            ;
        }
    }
}
void mcs_lock_release(struct mcs_node *node, int pid){
    //atomic equal
    struct mcs_node *tail = node;
    while(tail->next != NULL){
        tail = tail->next;
        if (tail->pid == pid){
            break;
        }
    }
    
    if(tail->next == NULL){
        return;
    }
    else
    {
        tail->next->is_locked = 0;
    }
    
}