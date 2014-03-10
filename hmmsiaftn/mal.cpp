#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <cassert>
#include <map>
#include "r3.h"
#include "tardis.h"
#include "mal.h"

using namespace std;

void kSort(double* begin, int size, int* kNN, int k);

bool compPtr (double* i, double* j){
    return *i < *j;
}

void mal(){
    string inputString, prevString;
    ifstream trainIn("train.mtx");
    getline(trainIn, inputString);
    int trainM, trainN, trainNnz;
    trainIn >> trainM >> trainN >> trainNnz;
    cerr << "train dimensions: " << trainM << " " << trainN << endl;
    vector<int> trainI, trainJ;
    vector<double> trainV;
    double tmpD;
    int tmpI0, tmpI1;
    for(int i = 0; i < trainNnz; i++){
        trainIn >> tmpI0 >> tmpI1 >> tmpD;
        trainI.push_back(tmpI0);
        trainJ.push_back(tmpI1);
        trainV.push_back(tmpD);
    }
    ifstream testIn("test.mtx");
    getline(testIn, inputString);
    int testN, testM, testNnz;
    testIn >> testM >> testN >> testNnz;
    vector<int> testI, testJ;
    vector<double> testV;
    for(int i = 0; i < testNnz; i++){
        testIn >> tmpI0 >> tmpI1 >> tmpD;
        testI.push_back(tmpI0);
        testJ.push_back(tmpI1);
        testV.push_back(tmpD);
    }

    ifstream trainClassIn("train.cls");
    int classesCount;
    trainClassIn >> classesCount >> tmpI0;
    string tmpS;
    vector<int> trainClass;
    for(int i = 0; i < trainM; i++){
        trainClassIn >> tmpI0 >> tmpS;
        trainClass.push_back(tmpI0);
    }
    vector<double> distance;
    vector<double> intermediateY;
    intermediateY.resize(trainM);
    for(int i = 0; i < intermediateY.size(); i++){
        intermediateY[i] = 0;
    }
    double* hardwareY = (double*)cny_cp_malloc(sizeof(double) * trainM);
    for(int i = 0; i < trainM; i++){
        hardwareY[i] = 0;
    }
    vector<double> hostY;
    hostY.resize(trainM);
    for(int i = 0; i < hostY.size(); i++){
        hostY[i] = 0;
    }
    vector<double> xVector;
    xVector.resize(trainN);
    for(int i = 0; i < xVector.size(); i++){
        xVector[i] = 0;}
    double* xVectorHardware = (double*)cny_cp_malloc(sizeof(double) * trainN);
    int currTestRow = 0;
    int tmp;
    spoonHeader* trainSpoon = cnySpoonFmt(&trainI[0], &trainJ[0], &trainV[0], trainM, trainN, trainNnz);
#ifdef DEBUG
    cerr << "start" << endl;
    for(int i = 0; i < testNnz; i++){
        if(testI[i] != currTestRow){
            cerr << "case: " << i << endl;
            tmp = 0;
            for(int j = 0; j < trainNnz; j++){
                intermediateY[trainI[j]] += trainV[j] * xVector[trainJ[j]];
                //if(trainV[j] * xVector[trainJ[j]] > .5)
                //    intermediateY[trainI[j]] += 1;
 
                //cerr << trainV[j] << endl;
                //cerr << xVector[trainJ[j]] << endl;
            }
            vector<double*> yPtrs;
            for(int j = 0; j < intermediateY.size(); j++){
                yPtrs.push_back(&intermediateY[0] + j);
            }
            stealTardis();
            sort(yPtrs.begin(), yPtrs.end(), compPtr);
            returnTardis();
            //TODO: only largest 10
            int j = yPtrs.size() - 10;
            if(j < 0)
                j = 0;
            for(; j < yPtrs.size(); j++){
                int diff = yPtrs[j] - &intermediateY[0];
                cerr << *yPtrs[j] << " " << diff << " " << trainClass[diff] << endl;
            }
            cny_cp_memcpy(xVectorHardware, &xVector[0], sizeof(double) * trainN);
            stealTardis();
            runR3(trainSpoon, &xVectorHardware[0], &hardwareY[0]);
            returnTardis();
            stealTardis();
            cny_cp_memcpy(&hostY[0], &hardwareY[0], trainM * sizeof(double));
            returnTardis();
            for(int j = 0; j < intermediateY.size(); j++){
                if(hardwareY[j] * 1.01 < intermediateY[j] || hardwareY[j] * .99 > intermediateY[j]){
                    cerr << "Mismatch at: " << i << endl;
                    break;
                }
            }
            for(int j = 0; j < xVector.size(); j++){
                xVector[j] = 0;
            }
            for(int j = 0; j < intermediateY.size(); j++){
                intermediateY[j] = 0;
            }
            currTestRow = testI[i];
        }else{
            xVector[testJ[i]] = testV[i];
        }
    }
#endif
    cerr << "running hardware" << endl;
    vector<double*> yPtrs;
    for(int j = 0; j < intermediateY.size(); j++){
        yPtrs.push_back(&hostY[0] + j);
    }
    stealTardis();
    cny_cp_free(trainSpoon);
    trainSpoon = cnySpoonFmt(&trainI[0], &trainJ[0], &trainV[0], trainM, trainN, trainNnz);
    currTestRow = 0;
    int firstRowElement = 0;
    for(int i = 0; i < trainN; i++){
        xVectorHardware[i] = 0;
    }
    int k = 30;
    vector<int> kNNIndices;
    for(int i = 0; i < k; i++)
        kNNIndices.push_back(-1);
    markTime();
    for(int i = 0; i < testNnz; i++){
        if(testI[i] != currTestRow){
            runR3(trainSpoon,&xVectorHardware[0], &hardwareY[0]);
            //TODO: software run
#ifndef TIME_CP
            cny_cp_memcpy(&hostY[0], &hardwareY[0], trainM * sizeof(double));

            kSort(&hostY[0], hostY.size(), &kNNIndices[0], k);
            cerr << "largest: " << kNNIndices[0] << endl;
            map<int, int> classificationCost;
            for(int j = 0; j < k; j++){
                if(kNNIndices[j] == -1)
                    continue;
                classificationCost[trainClass[kNNIndices[j]]] += hostY[kNNIndices[j]];
            }
            int maxClass = classificationCost.begin()->first;
            for(map<int, int>::iterator it = classificationCost.begin(); it != classificationCost.end(); it++){
            }
            //for(
            for(; firstRowElement < i; firstRowElement++){
                xVectorHardware[testJ[firstRowElement]] = 0;
            }
            for(int j = 0; j < k; j++)
                kNNIndices[j] = -1;
#endif
#ifdef DEBUG
            for(int j = 0; j < trainN; j++){
                assert(xVectorHardware[j] == 0);
            }
#endif
            currTestRow++;
        }else{
#ifndef TIME_CP
            xVectorHardware[testJ[i]] = testV[i];
#endif
        }
    }
    //TODO: last row
    runR3(trainSpoon,&xVectorHardware[0], &hardwareY[0]);
    returnTardis();

    trainIn.close();
    testIn.close();
    trainClassIn.close();
    cerr << "finished" << endl;
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
