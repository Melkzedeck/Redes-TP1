#pragma once
/*----------- Bliblioteca sockets POSIX -----------*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//-------------------------------------------------//
#include "Adress.h"


class Tclient {
	int socket_;
	static unsigned int tam_max;
public:
	//Construtor do socket com endereco
	Tclient(const Adress&);
	//Construtor copia
	Tclient(const Tclient&);
	//construtor a partir de um int
	Tclient(const int&);
	/* define um tamanho maximo de mensagem
	Tamanho default: 1024*/
	static void setTAM(const int&);
	//Get socket
	int sock() const;
	/* fluxo de saída de string:
	envia para o socket a string
	e retorna a qntd de bits enviadas*/
	ssize_t operator<<(const std::string&);
	/* fluxo de entrada de string:
	recebe do socket uma string
	e retorna a qntd de bits recebidas*/
	ssize_t operator>>(std::string&);
	//compara o numero do socket
	bool operator>(const Tclient&) const;
	//operador igual
	Tclient& operator=(const Tclient&);
	//destrutor
	~Tclient();
};

class STclient : public Tclient{
	Adress addr_;
public:
	STclient(int, Adress);
	STclient& operator=(const STclient&);
	Adress addr();
	std::string addr_str();
};

class Tserver {
	int socketS;
public:
	//Construtor do socket com endereco
	Tserver(Adress&);
	//Construtor copia
	Tserver(const Tserver&);
	//Get socket do Server
	int sockS() const;//Socket Server
	// Espera conexao e retorna o cliente conectado
	STclient waitConection();
	~Tserver();
};
