#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int N = 4;
const int steps = 10000000;
double f(double x) {
	return 4.0 / (1.0 + x * x);
}

int main(int argc, char* argv[]) {
	double pi = 0.0;
	const double dt = 1.0 / (double)steps;
	int i, nthr, first_go = 1;
	int chunk_size = 0, chunk_set = 10;
	int max_threads = omp_get_max_threads();
	if (N > max_threads)
		N = max_threads;
	omp_set_num_threads(N);
	omp_sched_t sched;//, sched_set = omp_sched_static;
	omp_set_schedule(atoi(argv[1]), chunk_set);

	// Pragma omp parallel for reduction

	double t = omp_get_wtime(); //tic
#pragma omp parallel for reduction (+:pi) schedule(runtime)
	for (i = 0; i < steps; i++) {
		if (first_go && omp_get_thread_num() == 0) {
			nthr = omp_get_num_threads();
			omp_get_schedule(&sched, &chunk_size);
			first_go = 0;
		}
		double x = dt * i;
		pi += f(x);
	}
	pi *= dt;
	t =  omp_get_wtime() - t; //toc

	// Pragma omp parallel + for + critical
	/*
	double t = omp_get_wtime(); //tic
#pragma omp parallel
{
	double pi_loc = 0.0;
#pragma omp for // schedule(dynamic, 2) <- nie działa
	for (i = 0; i < steps; i++) {
		if (first_go && omp_get_thread_num() == 0) {
			nthr = omp_get_num_threads();
			omp_get_schedule(&sched, &chunk_size);
			first_go = 0;
		}

		double x = dt * i;
		pi_loc += f(x) * dt;
	}
#pragma omp critical
	pi += pi_loc;
}
	t =  omp_get_wtime() - t; //toc
	*/

	printf("PI = %f \n", pi);
	printf("number of threads: \t %d \n", nthr);
	printf("time of execution: \t %f \n", t);
	printf("sched type: %d \t chunk size = %d \n", sched, chunk_size);
	printf("max number of threads: %d \t number of processors: %d",
			max_threads, omp_get_num_procs());
	return 0;
}
