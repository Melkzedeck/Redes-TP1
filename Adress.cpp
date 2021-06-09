#include "Adress.h"
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>

using std::string;
using std::stoi;
using std::to_string;
using std::cout;
using std::endl;
using std::invalid_argument;

void logexit(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

void usage_s(string name) {
    cout << "usage: " << name << " <v4|v6> <server port>" << endl;
    cout << "example: " << name << " v4 51511" << endl;
    exit(EXIT_FAILURE);
}

void usage_c(string name) {
    cout << "usage: " << name << " <server IP> <server port>" << endl;
    cout << "example: " << name << " 127.0.0.1 51511" << endl;
	exit(EXIT_FAILURE);
}

Adress::Adress(char v, string port_s){
	port_ = (uint16_t)stoi(port_s);
	memset(&storage, 0, sizeof(storage));
	addrlen_ = new socklen_t;
	if(v=='4'){
		addr6 = NULL;
		family_ = AF_INET;
		addr4 = (struct sockaddr_in *)&storage;
		addr4->sin_family = AF_INET;
        addr4->sin_addr.s_addr = INADDR_ANY;
        addr4->sin_port = htons(port_);
	}
	else if(v=='6'){
		addr4=NULL;
		family_ = AF_INET6;
		addr6 = (struct sockaddr_in6 *)&storage;
		addr6->sin6_family = AF_INET6;
        addr6->sin6_addr = in6addr_any;
        addr6->sin6_port = htons(port_);
	}
	else
		throw invalid_argument("versao desconhecida");
	addr_ = (struct sockaddr *)(&storage);
    *(addrlen_) = sizeof(storage);
}

Adress::Adress(string addrstr, string port_s){
	port_ = (uint16_t)stoi(port_s);
	const char *addrstr_ = addrstr.c_str();
    memset(&storage, 0, sizeof(storage));
    addrlen_ = new socklen_t;

	struct in_addr inaddr4;// IPv4 address
    struct in6_addr inaddr6; // IPv6 address
    if (inet_pton(AF_INET, addrstr_, &inaddr4)) {
        addr6 = NULL;
        addr4 = (struct sockaddr_in *)&storage;
        addr4->sin_family = AF_INET;
        addr4->sin_port = htons(port_);;
        addr4->sin_addr = inaddr4;
		family_ = AF_INET;
    }
    else if (inet_pton(AF_INET6, addrstr_, &inaddr6)) {
        addr4 = NULL;
        addr6 = (struct sockaddr_in6 *)&storage;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = htons(port_);
        addr6->sin6_addr = inaddr6;
        memcpy(&(addr6->sin6_addr), &inaddr6, sizeof(inaddr6));
		family_ = AF_INET6;
    }
    else
		throw invalid_argument("versao desconhecida");
    addr_ = (struct sockaddr *)(&storage);
    *(addrlen_) = sizeof(storage);
}

Adress::Adress(struct sockaddr *addr){
    memset(&storage, 0, sizeof(storage));
    addrlen_ = new socklen_t;
    if (addr->sa_family == AF_INET) {
        family_ = AF_INET;
        addr6 = NULL;
        addr4 = (struct sockaddr_in *)&storage;
        addr4->sin_family = AF_INET;
        struct sockaddr_in *aux = (struct sockaddr_in*)addr;
        addr4->sin_addr = aux->sin_addr;
        addr4->sin_port = aux->sin_port;
    }
    else if(addr->sa_family == AF_INET6){
        family_ = AF_INET6;
        addr4 = NULL;
        addr6 = (struct sockaddr_in6 *)&storage;
		addr6->sin6_family = AF_INET6;
        struct sockaddr_in6 *aux = (struct sockaddr_in6*)addr;
        addr6->sin6_addr = aux->sin6_addr;
        addr6->sin6_port = aux->sin6_port;

    }
    *(addrlen_) = sizeof(storage);
}

Adress::Adress(const Adress& aux){
    this-> port_ = aux.port_;
    this-> addr_ = aux.addr_;
	this-> addr4 = aux.addr4;
	this-> addr6 = aux.addr6;
	this-> storage = aux.storage;
	this-> addrlen_ = new socklen_t;
    *(this-> addrlen_) = *(aux.addrlen_);
	this-> family_ = aux.family_;
}

Adress::Adress(){
    port_ = 0;
    addr_ = NULL;
	addr4 = NULL;
	addr6 = NULL;
    addrlen_ = NULL;
	family_ = 0;
}

string Adress::str() const{
	int version;
    char addrstr[INET6_ADDRSTRLEN + 1] = "";
    if (family_ == AF_INET) {
        version = 4;
        if (!inet_ntop(AF_INET, &(addr4->sin_addr), addrstr,
                       INET6_ADDRSTRLEN + 1)) {
			logexit("ntop");
            cout<<"error";
        }
    } else if (family_ == AF_INET6) {
        version = 6;
        if (!inet_ntop(AF_INET6, &(addr6->sin6_addr), addrstr,
                       INET6_ADDRSTRLEN + 1)) {
            logexit("ntop");
            cout<<"error";
        }
    } else
		throw invalid_argument("versao desconhecida");

    string addrstr_s =  "IPv" + to_string(version) + " " + 
    string(addrstr) + " " + to_string((int)port_);
    return addrstr_s;
}

void Adress::refresh(){

    if (family_ == AF_INET) {
        port_ = ntohs(addr4->sin_port); // network to host short
    } else if (family_ == AF_INET6) {
        port_ = ntohs(addr6->sin6_port); // network to host short
    } else
		throw invalid_argument("versao desconhecida");
}

Adress& Adress::operator=(const Adress& aux){
    this-> port_ = aux.port_;
    this-> addr_ = aux.addr_;
	this-> addr4 = aux.addr4;
	this-> addr6 = aux.addr6;
	this-> storage = aux.storage;
	this-> addrlen_ = new socklen_t;
    *(this-> addrlen_) = *(aux.addrlen_);
	this-> family_ = aux.family_;
    return (*this);
}

bool Adress::empty(){
    bool emp1, emp2;
    emp1 = port_ == 0 && family_==0 && addrlen_==NULL;
    emp2 = addr_==NULL && addr4 == NULL && addr6 == NULL;
    return emp1 && emp2;
}


Adress::~Adress(){
    delete addrlen_;
}

