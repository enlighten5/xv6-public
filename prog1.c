// This test case is designed for stride scheduler
#include   "types.h"
#include   "user.h"

int main(int argc, char * argv[])
{

   int Scheduler(void);
   Scheduler();
   return 0 ;
 }


  int Scheduler(void){

  int pid;
  int i,j,k;

  //ticket(100);
  for(i = 0; i < 3;i++ ) {
    pid = fork ();
    if(pid > 0) {
      continue ;}
    else if (pid == 0){
      if(i == 0){ticket(100);}
      else if(i == 1){ticket(50);}
      else {ticket(250);}

     for(j = 0 ;j < 500000 ;j ++ ) {
       for (k = 0 ;k < 10000 ;k ++ ) {
         asm ( "nop" ); }}
     printf ( 1 ,  " \n  child# %d with %d tickets has finished!  \n " , getpid (), 30 - 10 * i);
     exit ();
   }
   else {
     printf ( 2 , "  \n  Error  \n " );
   }
 }

   if (pid  >   0 ) {
     for  (i  =   0 ; i  <   3 ; i ++ ) {
       wait ();

    }

}
   exit ();
   return   0 ;}
