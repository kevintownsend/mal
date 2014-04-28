#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]){
    cerr << argv[1] << endl;
    ifstream iFile(argv[1]);
    string line;
    stringstream sLine;
    string word;
    ofstream outputFile;
    int i = 0;
    while(getline(iFile, line)){
        istringstream sLine(line);
        sLine >> word;
        if(word == ""){
            outputFile << endl;
            cerr << "closing file" << endl;
            outputFile.close();
            break;
        }else if(word == ".I"){
            string fileName;
            sLine >> fileName;
            outputFile.close();
            outputFile.open("documents/a/" + fileName);
        }else if(word == ".W"){
            while(!sLine.eof()){
                sLine >> word;
                outputFile << word << " ";
            }
        }else{
            outputFile << word << " ";
            while(!sLine.eof()){
                sLine >> word;
                outputFile << word << " ";
            }
        }
        i++;
    }
    outputFile.close();
    return 0;
}
