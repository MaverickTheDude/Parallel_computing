#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CHILDREN 3

int main(int argc, char* argv[]) {
	char* arg;
	if (argc > 1)
		arg = argv[1];
	else
		arg = "90";
	int i = 0, status;
	int t[] = {2, 3, 3, 1, 5};
	pid_t child_pid;

	for (i = 0; i < CHILDREN; i++) {
		printf("\n");
		child_pid = fork();
		if (child_pid == 0) {
			// Proces potomny
			printf("proces potomny: PID: %d, PPID: %d \n",getpid(), getppid());
			sleep(t[i]);
			if (i == 1) {
				printf("proces potopmny: Uruchamiam zewnetrzny proces:\n");
				execl("test_project", "test_project", arg, NULL);
				printf("fail\n");
			}
			printf("proces potomny: %d - done \n", i);
			exit(0);
		}
		else if (child_pid != 0) {
			// Proces macierzysty
			printf("proces macierzysty: PID: %d, PPID: %d \n", getpid(), getppid());
			child_pid = wait(&status);
			printf("proces macierzysty: proc. potomny zakonczony, PID: %d, status: %d \n", child_pid, WEXITSTATUS(status));
		}
		else if (child_pid == -1) {
			perror("cos poszlo nie tak.\n");
		}
	}

	printf("\n\nProc. macierzysty konczy prace, PID: %d, PPID: %d \n", getpid(), getppid());

	return 0;
}
