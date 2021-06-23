#include <iostream>
#include <fstream>
#include "Adress.h"
#include "socket_TCP.h"

using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::ifstream;
using std::ofstream;


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
	ifstream file_in("./redes_tp1/tests/in/test_3.in");
	ofstream file_out("test_3.out");
	string input, msg;
	while(getline( file_in, input)){
		input +='\n';
		int num =sock<<input;
		if(strcasecmp(input.c_str(),"kill\n")==0)
			continue;
		num = sock >> msg;
		file_out << msg;
		cout << "< " << msg;
		input.clear();
		msg.clear();
	}
	cout << "Closing the program..." << endl;
	return 0;
}


