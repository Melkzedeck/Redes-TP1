#include "socket_TCP.h"

/*-------------------- Comeco da classe Tclient --------------------*/

unsigned int Tclient::tam_max=1024;

Tclient::Tclient(const Adress& addr){
	socket_ = socket(addr.family(), SOCK_STREAM, 0);
	if (socket_ == -1) {
		logexit("socket");
	}
	if (0 != connect(socket_, addr.addr(), *(addr.addrlen()))) {
		logexit("connect");
	}

}


ssize_t Tclient::operator<<(const std::string& str){
		return send(socket_, str.c_str(), str.size(), 0);
}

ssize_t Tclient::operator>>(std::string& str){
    char *msg;
    msg = new char[tam_max];
    ssize_t cnt = recv(socket_, msg, tam_max, 0);
    str = std::string(msg);
    delete msg;
    return cnt;
}

Tclient& Tclient::operator=(const Tclient& cli){
    this->socket_ = cli.socket_;
    return *this;
}

Tclient::~Tclient(){
	close(socket_);
}

/*-------------------- Termino da classe Tclient --------------------*/

/*-------------------- Comeco da classe STclient --------------------*/

STclient::STclient(int s, Adress addr1):Tclient(s){
    addr_ = addr1;
}

STclient& STclient::operator=(const STclient& cli){
    this->Tclient::operator=(cli);
    this->addr_ = cli.addr_;
    return *this;
}


/*-------------------- Termino da classe STclient --------------------*/


/*-------------------- Comeco da classe Tserver --------------------*/

Tserver::Tserver(Adress& addr){
	socketS = socket(addr.family(), SOCK_STREAM, 0);
	if (socketS == -1) {
        logexit("socket");
    }

    int enable = 1;
    if (0 != setsockopt(socketS, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
        logexit("setsockopt");
    }

    if (0 != bind(socketS, addr.addr(), *(addr.addrlen()))) {
        logexit("bind");
    }

    if (0 != listen(socketS, 10)) {
        logexit("listen");
    }
}


STclient Tserver::waitConection(){
    int socketC;
	struct sockaddr_storage cstorage;
    struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
    socklen_t caddrlen = sizeof(struct sockaddr_storage);
	socketC = accept(socketS, caddr, &caddrlen);
	if (socketC == -1) {
            logexit("accept");
        }
	return STclient(socketC,Adress(caddr));
}


Tserver::~Tserver(){
	close(socketS);
}

/*-------------------- Termino da classe Tserver --------------------*/

