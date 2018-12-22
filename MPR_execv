#include <stdio.h>
#include <process.h>
#include <unistd.h>
#include <stdlib.h>
//int argc, char* argv[]

// Uruchamia aplikacje bez zadnych argumenotw
// [DONE] To do: napisac aplikacje przyjmujaca argumenty,
// zeby mozna je bylo zadawac z konsoli przez funkcje execv
int main(int argc, char* argv[]) {
	int R, status;
	char* path = "/home/hi";
	char* args[argc-1];
	for(int i=0; i<argc-1 ;i++)
		args[i] = argv[i+1];
	R = fork();
	switch (R) {
		case -1:
			perror("M: Nie udalo sie utworzyc procesu potomnego...\n");
			break;
		case 0:
			execv(path,args);
			//execl("/home/hi","hi",NULL); // Okreslona liczba argumentow
			break;
		default:
			R = wait(&status);
			printf("MAT: Proces potomny zakonczony, PID = %d, \t Status = %d \n",
					R, WEXITSTATUS(status));
	}
	return 0;
}
