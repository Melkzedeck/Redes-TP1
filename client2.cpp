#include <fstream>
#include <iostream>
#include <string>
#include "./include/Adress.h"
#include "./include/socket_TCP.h"

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
    string line, msg;
    ifstream myfile("./redes_tp1/tests/in/test_3.in");
    ofstream output("test_3.out");
    if (myfile){
        string pct;
        while (getline( myfile, line )){
            line += '\n';
            cout<< "< " << line;
            int num =sock<<line;
            if(line=="kill\n")
                continue;
            do{
                num += sock>>pct;
                msg += pct;
            }while(pct.back()!='\n');
            cout << "> " << msg;
            output << msg;
		    msg.clear();
		    pct.clear();
        }
        myfile.close();
    }
    else cout << "fooey\n"; 
    cout << "Closing the program..." << endl;
    return 0;
  }
