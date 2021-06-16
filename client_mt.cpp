#include <iostream>
#include "Adress.h"
#include "socket_TCP.h"

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


/*Exemplo de um programa cliente*/
/*envio de mensagens simples*/

int main(int argc, char **argv) {
	
	usage(argc, argv);
	Adress server(argv[1], argv[2]);
	Tclient sock(server);
	cout<<"connected to " << server.str() << endl;
	string msg = "the first message";
	int num = sock<<msg;
	cout << "num of bytes sended: " << num << endl;
	num = sock>>msg;
	cout << "the message received: " << msg << endl;
	cout << "Closing the program..." << endl;
	return 0;
}
