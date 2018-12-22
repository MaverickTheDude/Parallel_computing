#include <stdio.h>
#include <process.h>
#include <unistd.h>
#include <stdlib.h>
#include "../newproc.h"
#include "../nextparent.h"

#define BEGIN 0

/* Zadania 3,4 - Rozbudowane drzewko z funkcji FORK() */

int main() {
	int procSize = 3;
	int status;
	int cycles[] = {3, 2, 5};

	int R[procSize];
	int* Rp[procSize];
	for (int i = 0; i < procSize; i++)
		Rp[i] = &R[i];
	nextparent(Rp, cycles, procSize, BEGIN);
	for (int i = 0; i < procSize; i++) {
		R[i] = wait(&status);
		printf("MAT: Proces potomny zakonczony, PID = %d, \t Status = %d \n",
						R[i], WEXITSTATUS(status));
	}

	/* Metoda na chama */

	//int itab[] = {1, 2};
//	int R1 = newproc(cycles[0],itab[0]);
//	if (R1 == -1)
//		perror("M: Nie udalo sie utworzyc procesu potomnego...\n");
//	else if (R1 != 0) { // Macierzysty
//
//		int R2 = newproc(cycles[1],itab[1]);
//		if (R2 == -1)
//			perror("M: Nie udalo sie utworzyc procesu potomnego...\n");
//		else if (R2 != 0) {
//			R1 = wait(&status);
//			printf("MAT: Proces potomny zakonczony, PID = %d, \t Status = %d \n",
//					R1, WEXITSTATUS(status));
//			R2 = wait(&status);
//			printf("MAT: Proces potomny zakonczony, PID = %d, \t Status = %d \n",
//					R2, WEXITSTATUS(status));
//			printf("MAT: Zakonczenie procesu macierzystego\n");
//		}
//		else if (R2 == 0) { // Potomny
//			printf("potomny: Zakonczenie procesu potomnego PID = %d\n", getpid());
//			exit(EXIT_SUCCESS);
//		}
//
//	}
//	else if (R1 == 0) { // Potomny
//		printf("potomny: Zakonczenie procesu potomnego PID = %d\n", getpid());
//		exit(EXIT_SUCCESS);
//	}

	printf("Proces PID = %d konczy prace \n", getpid());
	return 0;
}
