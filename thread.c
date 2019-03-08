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
