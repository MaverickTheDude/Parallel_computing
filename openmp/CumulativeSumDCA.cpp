/* https://stackoverflow.com/questions/18719257/
 * parallel-cumulative-prefix-sums-in-openmp-communicating-values-between-thread/65681828#65681828 */
#include<vector>
#include<iostream>
#include <omp.h>
#include <Eigen/Dense>
using namespace Eigen;
using std::cout;
using std::endl;

const int n          = 1000;
const int baseLength = 10;

Vector3d f(int ii) {
	Vector3d tmp = Vector3d::Constant(ii);
	return tmp;
}

Vector3d recursiveSumBody(Vector3d* begin, Vector3d* end){

	size_t length  = end - begin;
	size_t mid     = length/2;
	Vector3d sum   = Vector3d::Zero();

	if ( length < baseLength ) {
		for(size_t ii = 1; ii < length; ii++ ){
			begin[ii] += begin[ii-1];
		}
	} else {
#pragma omp task shared(sum)
		{
			sum = recursiveSumBody(begin,begin+mid);
		}
#pragma omp task
		{
			recursiveSumBody(begin+mid,end);
		}
#pragma omp taskwait

#pragma omp parallel for
		for(size_t ii = mid; ii < length; ii++) {
			begin[ii] += sum;
		}
	}
	return begin[length-1];
}


int main() {

	Vector3d tmp; tmp.setZero();
	std::vector<Vector3d> a(n, tmp);

#pragma omp parallel
{
#pragma omp for
	for (int i=0; i<n; i++)
		a[i] = f(i);

#pragma omp single
	{
	recursiveSumBody(&a[0],&a[n]);
	}
}

	for (int i = 0; i < 10; i++)
		cout << a[i][1] << endl;

	cout << n*(n-1)/2 << endl;
	cout << a[n-1] << endl;

	return 0;
}
