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

void logexit(const char *msg);
void usage_s(std::string name);
void usage_c(std::string name);


class Adress {
	uint16_t port_;
	struct sockaddr* addr_;
	struct sockaddr_in *addr4;
	struct sockaddr_in6 *addr6;
	struct sockaddr_storage storage;
	socklen_t *addrlen_;
	int family_;
public:
	Adress(char v, std::string port_s);
	Adress(std::string addrstr, std::string port_s);
	Adress(struct sockaddr* addr);
	Adress(const Adress&);
	uint16_t port()const {return port_;};
	const struct sockaddr* addr() const {return addr_;};
	const socklen_t* addrlen() const {return addrlen_;};
	int family() const {return family_;};
	std::string str() const;
	void refresh();
	Adress& operator=(const Adress&);
	~Adress();
};



