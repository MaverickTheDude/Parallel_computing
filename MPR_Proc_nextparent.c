#ifndef NEXTPARENT_H_
#define NEXTPARENT_H_
#include <stdio.h>
#include <process.h>
#include <unistd.h>
#include <stdlib.h>

void nextparent(int* Rp[], int cycles[],int size, int currentProc) {
	if (currentProc < size) {
		*Rp[currentProc] = fork();
		if (*Rp[currentProc] == -1)
			perror("M: Nie udalo sie utworzyc procesu potomnego...\n");
		else if (*Rp[currentProc] != 0) { // Macierzysty
			currentProc++;
			nextparent(Rp, cycles, size, currentProc);
		}
		else if (*Rp[currentProc] == 0) { // Potomny
			for(int i = 0; i < cycles[currentProc]; i++) {
				printf("PID: %d, numer procesu: %d, iteracja: %d\n", getpid(), currentProc, i);
				sleep(1);
			}
			exit(EXIT_SUCCESS);
		}
	}
	else if (currentProc == size) {
		printf("Ostatni proces wykonany, PID: %d\n", getpid());
		printf("PPID: %d\n", getppid() );
	}
	else
		printf("Unexpected WTF!");
}
#endif /* NEXTPARENT_H_ */
