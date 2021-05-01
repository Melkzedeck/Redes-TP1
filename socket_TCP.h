#pragma once
/*----------- Bliblioteca sockets POSIX -----------*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//-------------------------------------------------//
#include <stdio.h>
#include <unistd.h>
#include <string> /* memset() */
#include <string.h>
#include <iostream>   // std::cout
#include <stdexcept>
#include <vector>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <utility>      // std::pair
#include "Adress.h"

class Tclient {
	int socket_;
	bool atv();
public:
	//Construtor do socket com endereco
	Tclient(const Adress&);
	//Construtor copia
	Tclient(const Tclient& c){this->socket_ = c.socket_;};
	//Get socket
	int sock() const {return socket_;};
	/* fluxo de saída de string:
	envia para o socket a string
	e retorna a qntd de bits enviadas*/
	ssize_t operator<<(const std::string&);
	/* fluxo de entrada de string:
	recebe do socket uma string
	e retorna a qntd de bits recebidas
	TAMANHO_MAXIMO=1024*/
	ssize_t operator>>(std::string&);
	//destrutor
	~Tclient();
};

/*-- Implementacao de um servidor de conexao unica --*/ 
class Tserver {
	int socketS;
	int socketC;
	bool atv();
public:
	//Construtor do socket com endereco
	Tserver(Adress&);
	//Construtor copia
	Tserver(const Tserver& copia){this->socketS = copia.socketS; this->socketC = copia.socketC;};
	//Get socket do Client
	int sockC() const {return socketC;};//Socket Client
	//Get socket do Server
	int sockS() const {return socketS;};//Socket Server
	// Espera conexao com algum cliente
	Adress waitConection();
	//fecha a conexao
	void closeConection();
	/* fluxo de saída de string:
	envia para o socket a string
	e retorna a qntd de bits enviadas*/
	ssize_t operator<<(const std::string&);
	/* fluxo de entrada de string:
	recebe do socket uma string
	e retorna a qntd de bits recebidas
	TAMANHO_MAXIMO=1024*/
	ssize_t operator>>(std::string&);
	//Destrutor
	~Tserver();
};


/*-- Implementacao de um servidor de conexao multipla usando SELECT --*/ 
class TSmultiple{
	const int masterSock;
	int max_sd;
	std::vector<std::pair<Adress, int>> socks;
	fd_set readfds;
	static const std::string message;
public:
	//Construtor usando endereco
	TSmultiple(Adress&);
	/* estado de espera de abertura e fechamento 
	de conexao e recebimento de mensagens. 
	Fecha ao receber a mensagem KILL*/
	void wait_mode();
	//destrutor de objeto
	~TSmultiple();
};

