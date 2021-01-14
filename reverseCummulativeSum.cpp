/*https://stackoverflow.com/questions/18719257/
 * parallel-cumulative-prefix-sums-in-openmp-communicating-values-between-thread*/
#include <iostream>
#include <omp.h>
#include <Eigen/Dense>
#include <vector>
using namespace Eigen;
using std::cout;
using std::endl;

const int N = 10;
const int Nthr = 4;
float f(int i) {return (float)i;}

int main(void) {
	// Prosta wersja 1d
/*	omp_set_num_threads(Nthr);
	float* a = new float[N];
	float *suma = new float[Nthr+1];
	suma[0] = 0.0;
	float sum = 0.0;
#pragma omp parallel for schedule(static) firstprivate(sum)
	for (int i=0; i<N; i++) {
		sum += f(i);
		a[i] = sum;
		if (omp_get_thread_num() == 1)
			cout << "watek " << omp_get_thread_num() <<" i = " << i << endl;
		suma[omp_get_thread_num()+1] = sum;
	}
	// this for-loop is also a commulative sum, but it has only Nthr iterations
	for (int i=1; i<Nthr;i++)
		suma[i] += suma[i-1];
#pragma omp parallel for schedule(static)
	for(int i=0; i< N; i++) {
		a[i] += suma[omp_get_thread_num()];
	}
	for (int i=0; i<N; i++) {
		cout << a[i] << endl;
	}
	delete[] suma;
	int n = 95;
	cout << a[n] << endl << n*(n+1)/2 << endl;
	delete[] a;
	return 0;*/

	// Reverse cummulative sum na przykladzie Eigen-a

	MatrixXd vec = MatrixXd::Constant(3, N, 1.0);
	for (int i=1; i<N; i++)
		vec.col(i) +=  vec.col(i-1);
	vec.rightCols(1).setZero();
	MatrixXd res(3, N); res.setZero();
	Vector3d sum; sum.setZero();
	omp_set_num_threads(Nthr);
	MatrixXd suma(3, Nthr+1);
	suma.col(0).setZero();
# pragma omp parallel for schedule(static) firstprivate(sum)
	for (int i = N - 2; i >= 0; i--) {
		sum += vec.col(i);
		res.col(i) = sum;
		suma.col(omp_get_thread_num()+1) = sum;
	}

	for (int i=1; i<Nthr; i++)
		suma.col(i) += suma.col(i-1);

# pragma omp parallel for schedule(static)
	for (int i = N - 2; i >= 0; i--) {
		res.col(i) += suma.col(omp_get_thread_num());
	}

	cout <<suma.row(1) <<endl;
	cout << vec.row(1) << endl << res.row(1) << endl;
	return 0;
}
