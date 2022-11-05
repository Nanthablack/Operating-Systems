#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include "bakery.h"
#define NUMPROCESS 5 
void  increment(int *, int);
void  decrement(int *, int);

int main(int  argc, char *argv[])
{
     int    shmID;
     int *count;
     pid_t  pid;
     int    status;
     
     initializeBakery(); //initialized shared memory for Bakery's algorithm
     shmID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
     if (shmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     
     count = (int *) shmat(shmID, NULL, 0);
     if ((int) count == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     int i =0;
     *count = 5;
          	   
     for (i = 0; i < NUMPROCESS; i++) {       
     	pid = fork();
     	if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     	}
     	else if (pid == 0) {
	  recordPid(i, getpid());	
          if (i % 2) { //odd	
		increment(count, i);
	  }
          else { //even
		decrement(count, i);
          }  		 
          exit(0);
     	}
     }	
	 for(i =0; i < NUMPROCESS; i++) {
		wait(&status);
	 }	
     printf("The final value of count is %d\n", *count);	
     removeBakery(); //remove shared memory for Bakery's algorithm	
     //remove shared memory used by this program	
     shmdt((void *) count);
     shmctl(shmID, IPC_RMID, NULL);	
     exit(0);
}

void  increment(int *count, int i)
{
 	enterCriticalSection(i, NUMPROCESS);
	int temp = *count;
	temp++;
	sleep(rand() % 4);
	*count = temp;
	exitCriticalSection(i);    
}
void  decrement(int *count, int i)
{
 	enterCriticalSection(i, NUMPROCESS);
	int temp = *count;
	temp--;
	sleep(rand() % 4);
	*count = temp;
	exitCriticalSection(i);    
}


