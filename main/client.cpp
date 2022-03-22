#include <iostream>
#include "Adress.h"
#include "socket_TCP.h"


using std::cout;
using std::endl;
using std::string;
using std::cin;


int main(int argc, char **argv) {
	usage_c(argc, argv);
	Tclient::setTamMax(500);
	Adress server(argv[1], argv[2]);
	Tclient sock(server);//servidor conectado
	cout<<"connected to " << server.str() << endl; 
	string input, msg, pct;
	do{
		cout << "> ";
		getline(cin,input);
		input+='\n';
		int num =sock<<input; //envia a msg
		if(strcasecmp(input.c_str(),"kill\n")==0)
			continue;
		//recebe as msgs mesmo se estiverem particionadas
		do{
            num += sock>>pct;
            msg += pct;
        }while(pct.back()!='\n');
		cout << "< " << msg;
		input.clear();
		msg.clear();
		pct.clear();
	}while(strcasecmp(input.c_str(),"kill\n")!=0);//se "kill" for mandado na rede, ele quebra o loop
	cout << "Closing the program..." << endl;
	return 0;
}


