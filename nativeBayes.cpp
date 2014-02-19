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

using namespace std;

bool compPtr (double* i, double* j){
    return *i < *j;
}

int main(int argc, char* argv[]){
    string inputString, prevString;
    ifstream trainIn("train.mtx");
    getline(trainIn, inputString);
    int trainN, trainM, trainNnz;
    trainIn >> trainN >> trainM >> trainNnz;
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
    testIn >> testN >> testM >> testNnz;
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
    intermediateY.resize(trainN);
    for(int i = 0; i < intermediateY.size(); i++){
        intermediateY[i] = 0;
    }
    vector<double> hardwareY;
    hardwareY.resize(trainN);
    for(int i = 0; i < hardwareY.size(); i++){
        hardwareY[i] = 0;
    }
    vector<double> xVector;
    xVector.resize(trainM);
    for(int i = 0; i < xVector.size(); i++){
        xVector[i] = 0;}
    int currTestRow = 0;
    int tmp;
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
            sort(yPtrs.begin(), yPtrs.end(), compPtr);
            //TODO: only largest 10
            int j = yPtrs.size() - 10;
            if(j < 0)
                j = 0;
            for(; j < yPtrs.size(); j++){
                int diff = yPtrs[j] - &intermediateY[0];
                cerr << *yPtrs[j] << " " << diff << " " << trainClass[diff] << endl;
            }

            spoonHeader* trainSpoon = cnySpoonFmt(&trainI[0], &trainJ[0], &trainV[0], trainM, trainN, trainNnz);
            runR3(trainSpoon, &xVector[0], &hardwareY[0]);
            break;
        }else{
            xVector[testJ[i]] = testV[i];
        }
    }
    trainIn.close();
    testIn.close();
    trainClassIn.close();
    //TODO: figure this out
    cerr << "finished" << endl;
}
