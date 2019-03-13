#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "param.h"
#include "thread.h"

struct seq_lock_t lock;
static int result =0;
static int threadnum;
static int passnum;
static int workpid=0; //initialization

void* threadfunc();
int read_int(struct seq_lock_t *lock, int* readable);

int main (int argc , char * argv [])
{
    if (argc < 3) {
        printf(2, "Please enter thread number");
        exit();
    }

    threadnum =atoi(argv[1]);// num of threads is decided by the parameter passed by user
    passnum = atoi(argv[2]);

    uint start_tick = uptime();  
    printf(1, "start tick is %d\n", start_tick);

    seq_lock_init(&lock);
    int i;
    for (i = 0; i < threadnum; i++) {
        int r = thread_create(threadfunc, (void *)i);
        if (r == -1) {
            printf(2, "Error: thread_create failure");
            exit();
        }
    }

    for (; i > 0; i--) {
        wait();
    }

    uint finish_tick = uptime();
    printf(1, "End tick is %d\n", finish_tick);
    printf(1, "Ticks elaspsed is %d\n", finish_tick - start_tick);
    exit();
}

/*threadfunc is the function passed into threads
 * arg is parameter passed into the thread_create 
 * here arg is the id of every thread 
 * if the thread which gets the lock should not run, it will sleep and release the lock 
*/

void *threadfunc(void *arg)
{
    int pidnum = (int)arg;
    int _workpid, _result;
    for (;;)
    {
        // anderson_lock_acquire(&lock, &my_ticket);
        _workpid = read_int(&lock, &workpid);
        _result = read_int(&lock, &result);
        if (pidnum == _workpid && _result < passnum) //still need to do pass
        {
            seq_lock_acquire(&lock);
            result++;
            workpid++;
            if (workpid == threadnum) // we have reached the max thread num.
                workpid = 0;
            printf(1, "pass number no:%d, Thread %d is passing the token to ", result, pidnum);
            printf(1, " thread %d\n", workpid);
            seq_lock_release(&lock);
            // sleep(5);
        }
        else if (_result >= passnum)
        { //all the work has been completed
            // seq_lock_release(&lock, &my_ticket);
            exit();
        }
        else //This thread shoud not be working now.
        {
            // anderson_lock_release(&lock, &my_ticket);
            // sleep(5);
        }
    }

    //printf(1,"time to end\n");
    exit();
}

int read_int(struct seq_lock_t *lock, int* readable) {
    int ret, before, after;

    for (;;) {
        before = seq_lock_get_counter(lock);
        if (before % 2 != 0)
            continue;
        ret = *readable;
        after = seq_lock_get_counter(lock);
        if (after % 2 != 0)
            continue;
        if (before != after)
            continue;
        return ret;
    }
}