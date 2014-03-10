#include <iostream>
#include <deque>
#include <list>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <set>
#include <utility>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "mal.h"

using namespace std;

int main(int argc, char* argv[]){
    cerr << "started" << endl;
    int sock, connected, bytesRecieved;
    char sendData[1024], recvData[1024];

    struct sockaddr_in serverAddr, clientAddr;
    int sinSize;
    if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
}
