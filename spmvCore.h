
#include <vector>
#include <utility>

std::vector< std::vector< std::pair<int, double> > > findNearestNeihbors(int trainM, int trainN, int trainNnz, std::vector<int> &trainI, std::vector<int> &trainJ, std::vector<double> &trainV, int testN, int testM, int testNnz, std::vector<int> &testI, std::vector<int> &testJ, std::vector<double> &testV);

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
