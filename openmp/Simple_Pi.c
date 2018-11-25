#include <stdio.h>
#include <omp.h>

const int N = 5;
#define PAD 16					// 64 B L1 Cache line size (anti false sharing trick)
const int steps = 1000000;

double f(double x) {
	return 4.0 / (1.0 + x * x);
}

int main(void) {
	const double delta = 1.0 / (double)N;
	const double dx = N * delta / (double)steps;
	int i, nthreads;
	double sum = 0.0;
	double xtab[N][PAD];
	for (i = 0; i < N; i++)
		xtab[i][0] = 0.0;
	omp_set_num_threads(N);

	double t1 = omp_get_wtime(); //tic
	#pragma omp parallel
	{
		double x;
		int id = omp_get_thread_num();
		printf("id = \t %d \n", id);
		if (id == 0)
			nthreads = omp_get_num_threads();
		for (x = (double)id*delta; x < (double)(id+1)*delta; x += dx) {
			xtab[id][0] += f(x)*dx;
		}
	}
	double t = omp_get_wtime() - t1; //toc

	for (i = 0; i < N; i++)
		sum += xtab[i][0];

	printf("pi ==\\approx== \t %f \n", sum);
	printf("number of threads: \t %d \n", nthreads);
	printf("time = \t %f \n", t);
	for (i = 0; i < N; i++)
		printf("XTAB[%d] = %f \n", i, xtab[i][0]);
	return 0;
}
