# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h> // Zeby pozbyc sie warninga o roznych rozmiarach rzutowania
/* (intptr_t) zastepuje typ (void*) */

# define STEP 1
# define NUM_THREADS 8

typedef struct {
	int cnt; // numer watku
	int sum; // suma kontrolna
	const char * msg; // wiadomosc

} thread_data;

/* Funkcja wykonywana przez watek */
void* start_routine( void* arg) {
	int i, id;

	//Zad 1
	thread_data* dane_pointer = (thread_data*)arg;
	id = dane_pointer->cnt;

	/* Wyswietl nr przekazanego watku i krok */
	for(i = 0; i < STEP ; i++) {
		printf("\tWatek : %d, step = %d\n", id , i);
		printf("\tmessage: %s\n", dane_pointer->msg);
		//sleep(1);
	}
	dane_pointer->sum = id;
	//pthread_exit( (void*)(intptr_t)id );
	//pthread_exit(NULL);
	return NULL;
}

/* The main program . */
void* initialize(void* arg) {
	int i, rc;
	pthread_t thread_id[NUM_THREADS];
	void* status;
	pthread_attr_t attr;

	// Zad 1:
	thread_data dane[NUM_THREADS];
	char* messages[8];
	messages[0] = "English: Hello World !";
	messages[1] = "French: Bonjour , le monde !";
	messages[2] = "Spanish: Hola al mundo ";
	messages[3] = "Polski: Witaj swiecie !";
	messages[4] = "German: Guten Tag , Welt !";
	messages[5] = "Russian: Zdravstvytye , mir !";
	messages[6] = "Japan: Sekai e konnichiwa !";
	messages[7] = "Latin: Orbis , te saluto !";

	int check = 0;
	for (i=0; i < NUM_THREADS; i++) {
		check += i;
		dane[i].cnt = i;
		dane[i].sum = check;
		dane[i].msg = messages[i];
	}

	/* Zainicjalizowanie struktury atrybutow */
	pthread_attr_init( &attr );
	/* Ustawienie atrybut dolaczalnosci */
	pthread_attr_setdetachstate(&attr , PTHREAD_CREATE_JOINABLE );
	int policy;
	pthread_attr_getschedpolicy(&attr, &policy);
	int* ptr = (int*) arg;
	*ptr = policy;

	for(i = 0; i < NUM_THREADS ; i++) {
		/* Utworz nowy watek , ktory bedzie wykonywal funkcje start routine . */
		/* Przekaz argument i do watku */
		printf("Tworze watek nr: %d\n", i);
		rc = pthread_create(& thread_id[i], &attr , &start_routine , (void*)(dane+i) );
		/* W przypadku , gdy nie uda sie utworzyc watku */
		if (rc) {
			printf(" Blad. Kod powrotu z funkcji pthread create(): %d\n", rc);
			exit(1);
		}
	}

	pthread_attr_destroy( &attr );
	/* Oczekiwanie na watki potomne */
	for(i = 0; i < NUM_THREADS ; i++) {
		rc = pthread_join( thread_id[i], &status );
		if (rc) {
			printf (" Blad . Kod powrotu z funkcji pthread join (): %d\n",rc);
			exit(1);
		}
		printf (" Watek glowny dolaczyl watek %d o statusie %d\n", i, (int)(intptr_t)status );
	}
	//printf("sizeof int: %d, sizeof void*: %d\n",sizeof(int),sizeof(void*)); -> 4 vs 8 stad jest warning

	for (i = 0; i < NUM_THREADS; i++)
		printf("suma kontrolna %d: %d \n", i, dane[i].sum);

	printf(" Watek glowny zakonczony. Wyjscie \n");
	pthread_exit( NULL );
}

int main() {
	pthread_t thread_id;
	void* status;
	int policy_arg;
	int rc = pthread_create(&thread_id, NULL , &initialize , (void*) &policy_arg);
	if (rc) {
		printf (" Blad . Kod powrotu z funkcji pthread join (): %d\n",rc);
		exit(1);
	}

	rc = pthread_join(thread_id, &status);
	printf("strategia szeregowania watku master(): %d \n", policy_arg);
	printf ("Watek main() dolaczyl watek master(). Status: %d\n", (int)(intptr_t)status );
	return 0;
}
