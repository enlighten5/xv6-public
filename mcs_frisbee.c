#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "param.h"
#include "thread.h"


struct mcs_node *node;
static int result =0;
static int threadnum;
static int passnum;
static int workpid=0; //initialization
void* threadfunc();

int main (int argc , char * argv [])
{
  if (argc == 0){
      printf(2, "Please enter thread number");
      exit();
  }
  threadnum =atoi(argv[1]);// num of threads is decided by the parameter passed by user
  passnum = atoi(argv[2]);
  uint start_tick = uptime();  
  printf(1, "start tick is %d\n", start_tick);
  int i;
  mcs_lock_init(node);
  for(i=0;i<threadnum;i++)
  {
    int r=thread_create(threadfunc,(void *)i);
    if (r == -1)
    {
      printf(2, "Error: thread_create failure");
      exit();
    }
  }
  for(; i > 0; i--)
  {
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

void* threadfunc(void *arg)
{
	
  int pidnum = (int)arg;
  for(;;) 
  {
    int pid = getpid();
    mcs_lock_acquire(node, pid);
    if(pidnum==workpid&&result<passnum)//still need to do pass
    {
      result++;
      printf(1,"pass number no:%d, Thread %d is passing the token to ",result,pidnum);
      workpid++;
      if(workpid ==  threadnum)// we have reached the max thread num. 
        workpid = 0 ;	
      printf(1," thread %d\n",workpid);
      mcs_lock_release(node, pid);
      sleep(5);
    }
    else if(result >= passnum){ //all the work has been completed
      mcs_lock_release(node, pid);
      exit();
    }
    else //This thread shoud not be working now.
    {
      mcs_lock_release(node, pid);
      sleep(5);
    }
  }
	
  //printf(1,"time to end\n");
  exit();	
}


