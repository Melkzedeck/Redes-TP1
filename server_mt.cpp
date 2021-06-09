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
    Adress server(argv[1][1],argv[2]), newClient;
    TSmultiple sock(server);
    int sock_new = 0;
    string msg;
    cout<< "bound to " << server.str()<<", waiting connections" << endl;
    bool turnOff = true;
    while(turnOff){
        sock.wait();
        try{
            sock_new = sock.check_newConection(newClient);
            cout << "[log] New connection in socket " << sock_new << ": " << newClient.str() << endl;
        }catch(bool){
            sock.msg(msg);
            cout << msg << endl;
            if(msg.substr(6) == "kill" || msg.substr(6)=="KILL" || msg.substr(6)=="Kill")
                break;
        }
    }
    return 0;
}