/* simpleforkAndExec.c */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
int main(){
  pid_t pid;
  pid = fork();
  if (pid < 0) { /* error occurred */
     fprintf(stderr, "Fork Failed");
	 exit(-1);
  }
  else if (pid == 0) { /* child process */
		execlp("/bin/ls", "ls", NULL);
  }
  else { /* parent process */
		/* parent will wait for the child to complete */
    wait (NULL);
    printf ("Child Complete\n");
    exit(0);
  }
}
