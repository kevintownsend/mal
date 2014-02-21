#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include "r3.h"
#include "tardis.h"

using namespace std;

bool compPtr (double* i, double* j){
    return *i < *j;
}

int main(int argc, char* argv[]){
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
    cerr << "running hardware" << endl;
    //TODO: pure hardware run
    trainIn.close();
    testIn.close();
    trainClassIn.close();
    //TODO: figure this out
    cerr << "finished" << endl;
}
