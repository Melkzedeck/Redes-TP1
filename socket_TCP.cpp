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

void TSmultiple::wait_mode(){
	bool turn_off = false;
	while(!turn_off)
	{
		//clear the socket set
		FD_ZERO(&readfds);
	
		//add master socket to set
		FD_SET(masterSock, &readfds);
		max_sd = masterSock;
		int activity, new_socket;
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
		{
			printf("select error");
		}
			
		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(masterSock, &readfds))
		{
			struct sockaddr_storage cstorage;
    		struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
			socklen_t caddrlen = sizeof(struct sockaddr_storage);
			new_socket = accept(masterSock, caddr, &caddrlen);
			if (masterSock == -1) {
					logexit("accept");
				}
			std::pair<Adress, int> new_user(Adress(caddr), new_socket);
			socks.push_back(new_user);
			//inform user of socket number - used in send and receive commands
			std::cout << "New connection: " << new_user.first.str() << std::endl;

			//send new connection greeting message
			const char *ab = message.c_str();
			if( send(new_socket,ab, message.size(), 0) != (ssize_t)message.size() )
			{
				logexit("send");
			}
			std::cout << "Welcome message sent successfully" << std::endl;
		}
		//else its some IO operation on some other socket
		for (std::vector<std::pair<Adress, int>>::iterator it=socks.begin(); it!=socks.end(); ++it)
		{				
			if (FD_ISSET( it->second , &readfds))
			{
				
				//Check if it was for closing , and also read the
				//incoming message
				char *buffer;
				buffer = new char[1024];
				if (recv( it->second , buffer, 1024, 0) == 0)
				{
					//Somebody disconnected , get his details and print
					std::cout << "Host disconnected: " << it->first.str() << std::endl;
					//Close the socket and mark as 0 in list for reuse
					close( it->second );
					socks.erase(it);
					it--;
				}
					
				//Echo back the message that came in
				else
				{
					std::string msg(buffer);
					if(msg == "KILL" || msg == "kill" || msg == "Kill"){
						turn_off = true;
					}
					std::cout << "[msg] from " << it->first.str() << std::endl;
					std::cout <<  ">> " << msg << std::endl;
				}
				delete buffer;
			}
		}
	}
}

TSmultiple::~TSmultiple(){
	std::cout<<"close the sockets of clients" << std::endl;
	for (std::vector<std::pair<Adress, int>>::iterator it=socks.begin(); it!=socks.end(); ++it){
		close(it->second);
	}
	std::cout<< "close the socket master" << std::endl;
	close(masterSock);
}

