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

#define BUFSZ 1024

/*Exemplo de um programa cliente*/
/*envio de mensagens simples*/

int main(int argc, char **argv) {
	
	usage(argc, argv);
	Adress server(argv[1], argv[2]);
	Tclient sock(server);
	cout<<"connected to " << server.str() << endl;
	string msg;
	int num = sock>>msg;
	cout << msg << endl;
	cout << "mensagem>> ";
	cin >> msg;
	num = sock<<msg;
	cout << num << endl;
	return 0;
}
