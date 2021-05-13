#include "socket_TCP.h"

Tclient::Tclient(const Adress& addr){
	socket_ = socket(addr.family(), SOCK_STREAM, 0);
	if (socket_ == -1) {
		logexit("socket");
	}
	if (0 != connect(socket_, addr.addr(), *(addr.addrlen()))) {
		logexit("connect");
	}

}

bool Tclient::atv(){
	static int error, retval;
	static socklen_t len = sizeof (error);
	error = 0;
	retval = getsockopt (socket_, SOL_SOCKET, SO_ERROR, &error, &len);
	return (retval == 0 && error ==0);
}

ssize_t Tclient::operator<<(const std::string& str){
	if(this->atv())
		return send(socket_, str.c_str(), str.length(), 0);
	else
		return 0;
}

ssize_t Tclient::operator>>(std::string& str){

	if(this->atv()){
		char *msg;
		msg = new char[1024];
		ssize_t cnt = recv(socket_, msg, 1024 + 1, 0);
		str = std::string(msg);
		delete msg;
		return cnt;
	}
	else
		return 0;
}

Tclient::~Tclient(){
	std::cout << "close the socket" << std::endl;
	close(socket_);
}


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
	socketC = -1;
}

bool Tserver::atv(){
	static int error, retval;
	static socklen_t len = sizeof (error);
	error = 0;
	retval = getsockopt (socketC, SOL_SOCKET, SO_ERROR, &error, &len);
	return (retval == 0 && error ==0);
}

Adress Tserver::waitConection(){
	struct sockaddr_storage cstorage;
    struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
    socklen_t caddrlen = sizeof(struct sockaddr_storage);
	socketC = accept(socketS, caddr, &caddrlen);
	if (socketC == -1) {
            logexit("accept");
        }
	return Adress(caddr);
}

ssize_t Tserver::operator<<(const std::string& str){
	if(this->atv())
		return send(socketC, str.c_str(), str.length(), 0);
	else
		return 0;
}

ssize_t Tserver::operator>>(std::string& str){
	
	if(this->atv()){
		char *msg;
		msg = new char[1024];
		ssize_t cnt = recv(socketC, msg, 1024 + 1, 0);
		str = std::string(msg);
		delete msg;
		return cnt;
	}
	else
		return 0;	
}

void Tserver::closeConection(){

	close(socketC);
}

Tserver::~Tserver(){
	std::cout<<"close the sockets" << std::endl;
	close(socketS);
	close(socketC);
}

const std::string TSmultiple::message = "successful connection";

TSmultiple::TSmultiple(Adress& addr):masterSock(socket(addr.family(), SOCK_STREAM, 0)){
	int enable = 1;
    if (0 != setsockopt(masterSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))){ 
        logexit("setsockopt");
	}
	if (0 != bind(masterSock, addr.addr(), *(addr.addrlen()))) {
        logexit("bind");
    }
	if (0 != listen(masterSock, 10)) {
        logexit("listen");
    }
	max_sd = 0;
}



int TSmultiple::wait(){
		//clear the socket set
		FD_ZERO(&readfds);
	
		//add master socket to set
		FD_SET(masterSock, &readfds);
		max_sd = masterSock;
		int activity;
		//add child sockets to set
		for (std::vector<std::pair<Adress, int>>::iterator it=socks.begin(); it!=socks.end(); it++)
		{
			FD_SET( it->second , &readfds);
				
			//highest file descriptor number, need it for the select function
			if(it->second > max_sd)
				max_sd = it->second;
		}
	
		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR))
			std::cout<<"select error" << std::endl;
		return activity;
}

Adress TSmultiple::check_newConection(){
	//If something happened on the master socket ,
	//then its an incoming connection
	if (FD_ISSET(masterSock, &readfds))
	{
		struct sockaddr_storage cstorage;
		struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
		socklen_t caddrlen = sizeof(struct sockaddr_storage);
		int new_socket = accept(masterSock, caddr, &caddrlen);
		if (masterSock == -1)
			logexit("accept");
		//const char *ab = message.c_str();
		if( send(new_socket,message.c_str(), message.size(), 0) != (ssize_t)message.size() )
		{
			logexit("send");
		}
		Adress new_adress(caddr);
		socks.push_back(std::pair<Adress, int>(new_adress, new_socket));
		return new_adress;
	}
	else
		throw false;
}

std::string TSmultiple::msg(){
	//else its some IO operation on some other socket
	std::string msg_;
	for (std::vector<std::pair<Adress, int>>::iterator it=socks.begin(); it!=socks.end(); ++it){				
		if (FD_ISSET( it->second , &readfds)){
			//Check if it was for closing , and also read the
			//incoming message
			char *buffer;
			buffer = new char[1024];
			if (recv( it->second , buffer, 1024, 0) == 0)
			{
				//Somebody disconnected , get his details and print
				msg_ = "[log] Host disconnected: " + it->first.str();
				close( it->second );
				socks.erase(it);
			}
			else
				msg_ = "[msg] " + std::string(buffer);
			delete buffer;
			break;
		}
	}
	return msg_;
}




TSmultiple::~TSmultiple(){
	std::cout<<"close the sockets of clients" << std::endl;
	for (std::vector<std::pair<Adress, int>>::iterator it=socks.begin(); it!=socks.end(); ++it){
		close(it->second);
	}
	std::cout<< "close the socket master" << std::endl;
	close(masterSock);
}

