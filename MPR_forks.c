#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 3
#define CHILDREN 3

int main(void) {
	int i = 0, j, status;
	int t[] = {2, 5, 6};
	pid_t fork_pid, child_pid;

	// Struktura | -> | -> | -> | -> |
	printf("%d: [i=%d] I'm the root process.\n", getpid(), i);
	for(; i < CHILDREN; ++i)
	{
		fork_pid = fork();
		if(fork_pid != 0)
		{
			/* I've just become a parent */
			printf("%d: [i=%d] I've just born a child %d.\n", getpid(), i, fork_pid);
			break;
		}
		printf("%d: [i=%d] I'm the new child.\n", getpid(), i);
		/* Expecting my own child in next iteration (if it'll happen) */
	}

	if(i > 0) /* only non-root */
	{
		for(j = 0; j < t[i-1]; ++j)
		{
			printf("%d: [i=%d] starting cycle no. %d\n", getpid(), i, j+1);
			sleep(1);
		}
	}

	/* fork_pid != 0 for all processes, except one */
	if(fork_pid != 0)
	{
		/* Wait for my only child... */
		child_pid = wait(&status);
		printf("%d: [i=%d] child %d finished with status %d\n", getpid(), i, child_pid, WEXITSTATUS(status));
	}

	// Struktura  |
	//        | | | | |
	/*
	for (i = 0; i < N; i++) {
		child_pid = fork();
		if (child_pid == 0) {
			// Proces potomny
			printf("proces potomny, PID: %d, PPID: %d \n",getpid(), getppid());
			sleep(2);
			printf("\n potomny - done \n");
			exit(0);
		}
		else if (child_pid != 0) {
			// Proces macierzysty
			printf("Proc. macierzysty, PID: %d, PPID: %d \n", getpid(), getppid());
			child_pid = wait(&status);
			printf("Proc. macierzysty zakonczony, PID: %d, status: %d \n", child_pid, WEXITSTATUS(status));
		}
		else if (child_pid == -1) {
			perror("cos poszlo nie tak.\n");
		}
	}
	*/
	//printf("Proc. macierzysty konczy prace, PID: %d, PPID: %d \n", getpid(), getppid());

	return 0;
}
