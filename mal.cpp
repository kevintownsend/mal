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
#include "spmvCore.h"
#include "tardis.h"

using namespace std;

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

    vector<vector<pair<int, double> > > knnResults = findNearestNeihbors(trainM, trainN, trainNnz, trainI, trainJ, trainV, testN, testM, testNnz, testI, testJ, testV);

    trainIn.close();
    testIn.close();
    trainClassIn.close();
    cerr << "finished" << endl;
}
