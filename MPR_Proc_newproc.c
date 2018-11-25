#include <stdio.h>
#include <process.h>
#include <unistd.h>
#include <stdlib.h>

int newproc(int cycles,int itab) {
	int R;

	R = fork();
	if (R == -1)
		perror("M: Nie udalo sie utworzyc procesu potomnego...\n");
	else if(R != 0) { // Proces macierzysty

	}
	else if (R == 0) { // Proces potomny
		for(int i = 0; i < cycles; i++)
		{
			printf("PID: %d, numer procesu: %d, iteracja: %d\n", getpid(), itab, i);
			sleep(1);
		}
	}
	return R;
}
