#include <iostream>
#include "./include/Adress.h"
#include "./include/socket_TCP.h"

using std::cout;
using std::endl;
using std::string;
using std::cin;


void usage(int argc, char **argv) {
	if (argc < 3) {
		printf("usage: %s <server IP> <server port>\n", argv[0]);
		printf("example: %s 127.0.0.1 51511\n", argv[0]);
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char **argv) {
	Tclient::setTAM(500);
	usage(argc, argv);
	Adress server(argv[1], argv[2]);
	Tclient sock(server);
	cout<<"connected to " << server.str() << endl;
	string input, msg, pct;
	do{
		cout << "> ";
		getline(cin,input);
		input+='\n';
		int num =sock<<input;
		if(strcasecmp(input.c_str(),"kill\n")==0)
			continue;
		do{
            num += sock>>pct;
            msg += pct;
        }while(pct.back()!='\n');
		cout << "< " << msg;
		input.clear();
		msg.clear();
		pct.clear();
	}while(strcasecmp(input.c_str(),"kill\n")!=0);
	cout << "Closing the program..." << endl;
	return 0;
}


