#include <iostream>
#include "Adress.h"
#include "socket_TCP.h"

using std::cout;
using std::endl;
using std::string;

void usage(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: %s <v4|v6> <server port>\n", argv[0]);
        printf("example: %s v4 51511\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    
    usage(argc, argv);
    Adress server(argv[1][1],argv[2]);
    Tserver sock(server);
    cout<< "bound to " << server.str()<<", waiting connections" << endl;
    Adress client = sock.waitConection();
    cout << "[log] connection from " << client.str() << endl;
    string msg;
    int num = sock>>msg;
    cout << msg << endl;
    msg = "the first reply";
    num = sock << msg;
    cout << num << endl;
    cout << "[log] close connection of " << client.str() << endl;
    sock.closeConection();
    return 0;
}