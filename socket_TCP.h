#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string> /* memset() */
#include <string.h>
#include <iostream>   // std::cout
#include <stdexcept>
#include <vector>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <utility>      // std::pair
#include <map>
#include "Adress.h"

class Tclient {
	int socket_;
	bool atv();
public:
	Tclient(const Adress&);
	Tclient(const Tclient& c){this->socket_ = c.socket_;};
	int sock() const {return socket_;};
	ssize_t operator<<(const std::string&);
	ssize_t operator>>(std::string&);
	~Tclient();
};

class Tserver {
	int socketS;
	int socketC;
	bool atv();
public:
	Tserver(Adress&);
	Tserver(const Tserver& copia){this->socketS = copia.socketS; this->socketC = copia.socketC;};
	int sockC() const {return socketC;};//Socket Client
	int sockS() const {return socketS;};//Socket Server
	Adress waitConection();
	void closeConection();
	ssize_t operator<<(const std::string&);
	ssize_t operator>>(std::string&);
	~Tserver();
};

class TSmultiple{
	const int masterSock;
	int max_sd;
	std::vector<std::pair<Adress, int>> socks;
	fd_set readfds;
	static const std::string message;
public:
	TSmultiple(Adress&);
	//int send(const Adress&,const std::string);
	//int send(const std::string, const std::string);
	void wait_mode();
	~TSmultiple();
};



