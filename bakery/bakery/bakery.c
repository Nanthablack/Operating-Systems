#include  <sys/types.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <sys/wait.h>
#include "bakery.h"

static struct Memory *ptr;
static int shmID = 0;
void initializeBakery() {
     shmID = shmget(IPC_PRIVATE, sizeof(struct Memory), IPC_CREAT | 0666);
     if (shmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     ptr = (struct Memory *) shmat(shmID, NULL, 0);
     if (ptr == NULL) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
}
void removeBakery() {
     shmdt((void *) ptr);
     shmctl(shmID, IPC_RMID, NULL);
}
void recordPid(int i, pid_t pid) {
	ptr->pid[i] = pid;
}
int max(int *number, int numProcess) {
	int i =0;
	int max = 0;
	for (i = 0; i < numProcess; i++) {
		if (number[i] > max) {
			max = number[i];
		}	
	}
	return max;
}
int checkOrder(int i, int j, pid_t pidi, pid_t pidj) {
	if(j < i) {
		return TRUE;
	} 
	if (j == i) {
		if (pidj < pidi) {
			return TRUE;
		}
	}
	return FALSE;
}

void enterCriticalSection(int i, int numProcess) {
	int j = 0;
	ptr->choosing[i] = TRUE;
	ptr->number[i] = max(ptr->number, numProcess) + 1;
	ptr->choosing[i] = FALSE; 
	for(j =0; j < numProcess; j++) {
		while(ptr->choosing[j]);
		while(ptr->number[j] != 0 && 
			checkOrder(ptr->number[i],ptr->number[j], ptr->pid[i], 
					ptr->pid[j]));					
	} 

}

int exitCriticalSection(int i) {

	ptr->number[i] = 0;
}
