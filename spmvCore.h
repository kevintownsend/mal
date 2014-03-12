
#include <vector>
#include <utility>

using namespace std;
vector<vector<pair<int, double> > > findNearestNeihbors(int trainM, int trainN, int trainNnz, vector<int> &trainI, vector<int> &trainJ, vector<double> &trainV, int testN, int testM, int testNnz, vector<int> &testI, vector<int> &testJ, vector<double> &testV);

/*
#if ENGINE == MKL
#include "mkl_types.h"
#include "mkl_spblas.h"
#elif ENGINE == CNY
#include "r3.h"
#elif ENGINE == BOOST
#include <boost/numeric/ublas/vector.hpp>
#elif ENGINE == NONE
#else
#endif

#if ENGINE == MKL
struct SpMatrix_t {
    int* rows;
    int* columns;
    double* value;
    int M, N, nnz;
};
#elif ENGINE == CNY
#elif ENGINE == BOOST
#elif ENGINE == NONE
struct SpMatrix_t {
    int* rows;
    int* columns;
    double* value;
    int M, N, nnz;
};
#else
#endif

#if ENGINE == MKL
#elif ENGINE == CNY
#elif ENGINE == BOOST
#elif ENGINE == NONE
#else
#endif
*/
