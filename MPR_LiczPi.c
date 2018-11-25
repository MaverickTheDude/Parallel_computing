#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdint.h> // (intptr_t)

void* calculateSum(void* args);
void* calculateProd(void* args);
void* calcOneBarrier();
void* calcOneCondvar();

#define SUMUJEMY 0 // set 0 for calculateProd
#define N 1e7
#define threads 2
double dx = 1/(double)N;
double sum; // or product
double factor;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_barrier_t barrier = PTHREAD_BARRIER_INITIALIZER(threads+1);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
	void* (*fun)(void*);
	fun = SUMUJEMY == 1 ? &calculateSum : &calculateProd;
	sum = SUMUJEMY == 1 ? 0.0 : 1.0;
	factor = SUMUJEMY == 1 ? dx : 2.0;

	int i, rc, rcl;
	void* status;
	pthread_t thread_id[threads];
	pthread_t last_thread_id;

	rcl = pthread_create(&last_thread_id, NULL , &calcOneCondvar , NULL );

	/* Utworz watki liczace swoja czesc funkcji */
	for(i = 0; i < threads ; i++) {
		rc = pthread_create(thread_id+i, NULL , fun, (void*)(intptr_t)i );
		if (rc) {
			printf(" Blad. Kod powrotu z funkcji pthread create(): %d\n", rc);
			exit(1);
		}
	}

	/* Dolacz wszystkie watki */
	for(i = 0; i < threads ; i++) {
		rc = pthread_join(thread_id[i], &status);
		if (rc) {
			printf (" Blad . Kod powrotu z funkcji pthread join (): %d\n",rc);
			exit(1);
		}
		printf(" Watek glowny dolaczyl watek %d o statusie %d\n", i, (int)(intptr_t)status );
	}

	// zmienna warunkowa:
	printf("main: dzwonie! \n");
	pthread_cond_signal(&cond);

	rcl = pthread_join(last_thread_id, &status);
	if (rcl) {
		printf (" Blad . Kod powrotu z funkcji pthread join (): %d\n",rc);
		exit(1);
	}
	printf(" Watek glowny dolaczyl ostatni watek o statusie %d\n", (int)(intptr_t)status );

	double pi = factor * sum;
	printf("pi = %f\n", pi);
	return EXIT_SUCCESS;
}

// https://en.wikipedia.org/wiki/Wallis_product
void* calculateProd(void* args) {
	int section = (int)(intptr_t)args;
	double n, f;
	int block = N/threads;

	for (int i=1; i < block+1; i++) {
		n = (double)(i + section * block);
		f = 4.0*n*n / (4.0*n*n-1.0);
		pthread_mutex_lock(&mutex);
		sum *= f;
		pthread_mutex_unlock(&mutex);

		//printf("watek %d \t krok %d \t n = %f \t f = %f \n", section, i, n, f);
	}
//	Bariery:
//	printf("watek %d: czekam na barierze \n", section);
//	pthread_barrier_wait(&barrier);
//	printf("watek %d: zlamalem bariere \n", section);
	pthread_exit(args); // Argumentem jest id watku
}

void* calculateSum(void* args) {
	int section = (int)(intptr_t)args;
	double x0 = (1.0/(double)threads) * (double)section;
	int status;

	double x, y;
	for (int i=0; i < N/threads; i++) {
		x = x0 + dx*(double)i;
		y = 4.0 / (1.0 + x*x);
		//status = pthread_mutex_lock(&mutex);
		sum += y;
		//status = pthread_mutex_unlock(&mutex);

		//printf("watek %d \t krok %d \t x = %f \t y = %f \n", section, i, x, y);
	}
//	Bariery:
//	printf("watek %d: czekam na barierze \n", section);
//	pthread_barrier_wait(&barrier);
//	printf("watek %d: zlamalem bariere \n", section);
	pthread_exit(args); // Argumentem jest id watku
}

void* calcOneBarrier() {
	printf("final: czekam na barierze...\n");
	pthread_barrier_wait(&barrier);
	printf("final: zlamalem bariere \n");
	double pi = factor * sum;
	double x = tan(pi/4);

	printf("1 = %f \n\n", x);

	pthread_exit(NULL);
}

void* calcOneCondvar() {
	int status;

	status = pthread_mutex_lock(&mutex);

	printf("final: czekam gwizdek...\n");
	pthread_cond_wait(&cond ,&mutex);
	printf("final: uslyszalem! \n");
	double pi = factor * sum;

	status = pthread_mutex_unlock(&mutex);

	double x = tan(pi/4);
	printf("1 = %f \n\n", x);
	pthread_exit(NULL);
}
