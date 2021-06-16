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
    STclient client = sock.waitConection();
    cout << "[log] connection from " << client.addr_str() << endl;
    string msg;
    int num = client>>msg;
    cout << "the messange received: "<< msg << endl;
    msg = "the first reply";
    num = client << msg;
    cout << "num of bytes sended: " <<num << endl;
    cout << "[log] close connection of " << client.addr_str() << endl;
    return 0;
}