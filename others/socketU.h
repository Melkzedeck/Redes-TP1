#pragma once
#include "Adress.h"

/*-------- ATENCAO!! --------*/
/*-------- Construcao da classe ainda em andamento --------*/

struct Data{
	void* ptr;
	unsigned long int size;
};

class SocketU{
	int sock;
	bool server;
	sockaddr *emissor_, *receptor_;
	socklen_t *emissor_l, *receptor_l;
public:
	SocketU(bool serv);
	int bind_s(Adress serv);
	void receptor(Adress em){receptor_ = em.addr(); receptor_l = em.addrlen();};
	ssize_t operator<<(std::string s);
	ssize_t operator<<(struct Data d);
	ssize_t operator>>(std::string s);
	ssize_t operator>>(struct Data d);
	struct sockaddr* last();
	~SocketU(){close(sock);};
};
