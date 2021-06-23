#include <fstream>
#include <iostream>
#include <string>
#include "Adress.h"
#include "socket_TCP.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

void usage(int argc, char **argv) {
	if (argc < 3) {
		printf("usage: %s <server IP> <server port>\n", argv[0]);
		printf("example: %s 127.0.0.1 51511\n", argv[0]);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
  {
    Tclient::setTAM(500);
	usage(argc, argv);
	Adress server(argv[1], argv[2]);
	Tclient sock(server);
	cout<<"connected to " << server.str() << endl;
    string line;
    ifstream myfile("./redes_tp1/tests/in/test_3.in");
    ofstream output("test_3.out");
    if (myfile){
        while (getline( myfile, line )){
            string msg;
            int num = 0;
            line += '\n';
            cout<< "< " << line;
            for(unsigned int i = 0; i < line.size(); i = i+2){
                num =sock<<line.substr(i,2);
            }
            if(line=="kill\n")
                continue;
            msg.clear();
            num = 0;
            cout << "> " << msg << "size of message: " << num << "\t size of request: " << msg.size() << endl;
            num = sock>>msg;
            output << msg;
        }
        myfile.close();
    }
    else 
        cout << "fooey\n"; 
    cout << "Closing the program..." << endl;
    return 0;
  }
