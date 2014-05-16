#include "spmvCore.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <stdint.h>
#include <cassert>
#include <map>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

void kSort(double* begin, int size, int* kNN, int k);

bool compPtr (double* i, double* j){
    return *i < *j;
}

void spmv (vector<int>::iterator aRows, vector<int>::iterator aColumns, vector<double>::iterator aValues, vector<double>::iterator y, vector<double>::iterator x, int start, int end);
void spmvParallel (vector<int> &aRows, vector<int> &aColumns, vector<double> &aValues, vector<double> &y, vector<double> &x);

vector<vector<pair<int, double> > > findNearestNeihbors(int trainM, int trainN, int trainNnz, vector<int> &trainI, vector<int> &trainJ, vector<double> &trainV, int testN, int testM, int testNnz, vector<int> &testI, vector<int> &testJ, vector<double> &testV){
    int threadCount = thread::hardware_concurrency();
    vector<int> rowCuts;
    rowCuts.resize(threadCount + 1);
    double spmvTime = 0;
    double sorting = 0;
    int k = 30;
    if(trainM < k)
        k = trainM;
    vector<vector<pair<int, double> > > ret;
    ret.resize(testM);
    vector<double> intermediateY;
    intermediateY.resize(trainM);
    for(int i = 0; i < intermediateY.size(); i++){
        intermediateY[i] = 0;
    }
    vector<double> xVector;
    xVector.resize(trainN);
    for(int i = 0; i < xVector.size(); i++){
        xVector[i] = 0;}
    int currTestRow = 0;
    int firstRowElement = 0;
    vector<int> kNNIndices;
    for(int i = 0; i < k; i++)
        kNNIndices.push_back(-1);
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(int i = 0; i < testNnz; i++){
        if(testI[i] != currTestRow){
            high_resolution_clock::time_point before = high_resolution_clock::now();
            for(int j = 0; j < intermediateY.size(); j++){
                intermediateY[j] = 0;
            }
#ifdef PARALLEL
            spmvParallel(trainI, trainJ, trainV, intermediateY, xVector);
            high_resolution_clock::time_point after = high_resolution_clock::now();
            //TODO: check result
#ifdef DEBUG
            vector<double> checkY;
            checkY.resize(intermediateY.size());
            for(int j = 0; j < trainNnz; j++){
                checkY[trainI[j]] += trainV[j] * xVector[trainJ[j]];
            }
            assert(checkY.size() != 0);
            for(int j = 0; j < checkY.size(); j++){
                if(checkY[j] != intermediateY[j]){
                    cerr << "ERROR: " << endl;
                    cerr << "check: " << checkY[j] << endl;
                    cerr << "intermediateY: " << intermediateY[j] << endl;
                }
                assert(checkY[j] == intermediateY[j]);
            }
#endif
#else
            for(int j = 0; j < trainNnz; j++){
                intermediateY[trainI[j]] += trainV[j] * xVector[trainJ[j]];
            }
            high_resolution_clock::time_point after = high_resolution_clock::now();
#endif

            spmvTime += duration_cast<duration<double> >(after-before).count();
            before = high_resolution_clock::now();
            kSort(&intermediateY[0], intermediateY.size(), &kNNIndices[0], k);
            after = high_resolution_clock::now();
            sorting += duration_cast<duration<double> >(after-before).count();
            for(int j = 0; j < k; j++){
                ret[currTestRow].push_back(make_pair(kNNIndices[j],intermediateY[kNNIndices[j]]));
            }
            for(; firstRowElement < i; firstRowElement++){
                xVector[testJ[firstRowElement]] = 0;}
#ifdef DEBUG
            for(int j = 0; j < xVector.size(); j++){
                assert(xVector[j] == 0);
            }
#endif
            for(int j = 0; j < k; j++)
                kNNIndices[j] = -1;
            currTestRow = testI[i];
        }
        xVector[testJ[i]] = testV[i];
    }
    high_resolution_clock::time_point before = high_resolution_clock::now();
    for(int j = 0; j < intermediateY.size(); j++){
        intermediateY[j] = 0;
    }
#ifdef PARALLEL
    cerr << "before parallel call" << endl;
    spmvParallel(trainI, trainJ, trainV, intermediateY, xVector);
    cerr << "after parallel call" << endl;
#else
    for(int j = 0; j < trainNnz; j++){
        intermediateY[trainI[j]] += trainV[j] * xVector[trainJ[j]];
    }
#endif
    high_resolution_clock::time_point after = high_resolution_clock::now();
    spmvTime += duration_cast<duration<double> >(after-before).count();
    before = high_resolution_clock::now();
    kSort(&intermediateY[0], intermediateY.size(), &kNNIndices[0], k);
    after = high_resolution_clock::now();
    sorting += duration_cast<duration<double> >(after-before).count();
    for(int j = 0; j < k; j++){
        ret[currTestRow].push_back(make_pair(kNNIndices[j],intermediateY[kNNIndices[j]]));
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
    cerr << "Classification took: " << time_span.count() << " seconds." << endl;
    cerr << "SpMV took: " << spmvTime << " seconds." << endl;
    cerr << "sorting took: " << sorting << " seconds." << endl;
    //TODO: find seg fault
    return ret;
}

void kSort(double* begin, int size, int* kNN, int k){
    if(k > size)
        k = size;
    for(int i = 0; i < size; i++){
        if(kNN[k-1] == -1){
        }else if(begin[i] < begin[kNN[k-1]])
            continue;
        bool largest = true;
        for(int j = k - 2; j >= 0; j--){
            if(kNN[j] == -1){
            }else if(begin[i] > begin[kNN[j]]){
                kNN[j+1] = kNN[j];
            }else{
                kNN[j + 1] = i;
                largest = false;
                break;
            }
        }
        if(largest)
            kNN[0] = i;
    }
}
void spmvParallel (vector<int> &aRows, vector<int> &aColumns, vector<double> &aValues, vector<double> &y, vector<double> &x){
    int threadCount = thread::hardware_concurrency();
    //cerr << "threadCount: " << threadCount << endl;
    threadCount = 20;
    if(threadCount == 0){
        spmv(aRows.begin(), aColumns.begin(), aValues.begin(), y.begin(), x.begin(), 0, aRows.size());
        return;
    }
    vector<int> rowCuts;
    rowCuts.resize(threadCount + 1);
    rowCuts[0] = 0;
    rowCuts[threadCount] = aRows.size();
    for(int i = 1; i < threadCount; i++){
        int j = aRows.size()*i / threadCount;
        int cutRow = aRows[j];
        if(j != aRows.size())
            while(cutRow == aRows[j++]){
                if(j == aRows.size())
                    break;
            }
        rowCuts[i] = j;

    }
    for(int i = 0; i < threadCount+1; i++){
    }
    vector<thread> threads;
    for(int i = 0; i < threadCount; i++){
        threads.push_back(thread(spmv, aRows.begin(), aColumns.begin(), aValues.begin(), y.begin(), x.begin(), rowCuts[i], rowCuts[i+1]));
    }
    for(int i = 0; i < threadCount; i++)
        threads[i].join();
}
void spmv (vector<int>::iterator aRows, vector<int>::iterator aColumns, vector<double>::iterator aValues, vector<double>::iterator y, vector<double>::iterator x, int start, int end){
    for(int i = start; i < end; i++){
        y[aRows[i]] += aValues[i] * x[aColumns[i]];
    }
}
