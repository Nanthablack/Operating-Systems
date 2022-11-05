#include <sys/types.h>
#define TRUE 1
#define FALSE 0
#define MAXPROCESS 5
struct Memory {
	int  number[MAXPROCESS];
	int choosing[MAXPROCESS];
	pid_t pid[MAXPROCESS];
};

