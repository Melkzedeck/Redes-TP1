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
    TSmultiple sock(server);
    cout<< "bound to " << server.str()<<", waiting connections" << endl;
    sock.wait_mode();
    cout << "turn off server " << endl;
    return 0;
}