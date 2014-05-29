#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include "spmvCore.h"

using namespace std;

int main(int argc, char* argv[]){
    //read train matrix
    ifstream trainMtxFile("train.mtx");
    vector<int> rows;
    vector<int> columns;
    vector<double> values;
    string line;
    stringstream lStream;
    getline(trainMtxFile, line);
    getline(trainMtxFile, line);
    lStream << line;
    int M, N, nnz;
    lStream >> M >> N >> nnz;
    for(int i = 0; i < nnz; i++){
        getline(trainMtxFile, line);
        stringstream lStream2;
        lStream2 << line;
        int tmp1, tmp2;
        double tmp3;
        lStream2 >> tmp1 >> tmp2 >> tmp3;
        rows.push_back(tmp1 - 1);
        columns.push_back(tmp2 - 1);
        values.push_back(tmp3);
    }
    trainMtxFile.close();
    cerr << "Hello World" << endl;
    //TODO: read query matrix
    int tmp1;
    double tmp2;
    vector<int> testRows;
    vector<int> testColumns;
    vector<double> testValues;
    for(int i = 0; i < N; i++){
        testRows.push_back(0);
        testColumns.push_back(i);
        testValues.push_back(1);
    }
    //TODO: read money
    //TODO: service
    //TODO: find segfault
    
    vector< vector< pair<int, double> > > results = findNearestNeihbors(M, N, nnz, rows, columns, values, 1, N, testValues.size(), testRows, testColumns, testValues);
    cerr << "leaving main" << endl;
    cerr << "Test vector: " << endl;
    for(int i = 0; i < testValues.size();i++){
        cerr << testRows[i] << " " << testColumns[i] << " " << testValues[i] << endl;
    }
}
