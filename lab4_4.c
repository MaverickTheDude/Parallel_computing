#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CHILDREN 3

int main(int argc, char* argv[]) {
	int i = 0, status;
	int t[] = {2, 3, 3, 1, 5};
	pid_t child_pid;
	pid_t main_pid = getpid();
	char* test = (char*) malloc(100*sizeof(char));
	sprintf(test,"wartosc: glowny");

	for (i = 0; i < CHILDREN; i++) {
		child_pid = fork();

		if (child_pid == 0) {
			// Proces potomny
			printf("proces potomny: PID: %d, PPID: %d \n",getpid(), getppid());
			sleep(t[i]);
			if (i == CHILDREN-1) {
				printf("proces %d: Uruchamiam zewnetrzny proces:\n", getpid());
				execl("/data/home/user1/just_hello", "xxx", NULL);
				printf("fail\n");
			}
		}
		else if (child_pid != 0) {
			// Proces macierzysty
			printf("proces macierzysty: PID: %d, PPID: %d \n", getpid(), getppid());
			sprintf(test, "%d zmienilem", (long) getpid());
			child_pid = wait(&status);
			printf("proces macierzysty %d: proc. potomny zakonczony, PID: %d, status: %d \n", getpid(), child_pid, WEXITSTATUS(status));
			if (getpid() == main_pid)
				break;
			else
				exit(0);
		}
		else if (child_pid == -1) {
			perror("cos poszlo nie tak.\n");
		}
	}

	printf("%s\n", test);
	printf("\n\nProc. macierzysty konczy prace, PID: %d, PPID: %d \n", getpid(), getppid());

	return 0;
}
