#pragma once
//----------- Bibliotecas interface POSIX -----------//
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//----------------------////----------------------//
#include <stdio.h>
#include <unistd.h>
#include <string> /* memset() */
#include <string.h>
#include <stdexcept> //Tratamento de algumas excecoes

void logexit(const char *msg); //saida de erro
void usage_s(int argc, char **argv);
void usage_c(int argc, char **argv);

/* Classe para tratamento de endereco e porta. A implementacao é baseada 
na interface POSIX (desenvolvida nessa classe em C) de soquetes de rede*/

class Adress {
	uint16_t port_;
	struct sockaddr* addr_;
	struct sockaddr_in *addr4;
	struct sockaddr_in6 *addr6;
	struct sockaddr_storage storage;
	socklen_t *addrlen_;
	int family_;
public:
	//Construtor da classe usando a versão(char: '4'-(IPv4) ou '6'-(IPv6)) e a porta(string) como argumento
	Adress(char v, std::string port_s);
	//Construtor usando o endereco(string) e porta(string)
	Adress(std::string addrstr, std::string port_s);
	//Construtor usando ponteiro do struct sockaddr
	Adress(struct sockaddr* addr);
	//Construtor copia
	Adress(const Adress&);
	// Construtor padrao
	Adress();
	//----------- Funcoes GET -----------//
	uint16_t port() const;
	const struct sockaddr* addr() const;
	const socklen_t* addrlen() const;
	//retorna a familia do endereco (IPv4 ou IPv6)
	int family() const;
	//----------- Termino das funcoes GET -----------//

	//retorna o endereco no formato string
	std::string str() const;
	//operador de igualdade 
	Adress& operator=(const Adress&);
	//verifica se é um endereco vazio
	bool empty();
	//Destrutor (alocacao de memoria)
	~Adress();
};



