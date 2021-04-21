#include "socketU.h"
using std::string;
SocketU::SocketU(bool serv){
    server = serv;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock<0){
        std::runtime_error("socket create failed");
    }
    emissor_ = NULL;
    receptor_ = NULL;
    emissor_l = NULL;
    receptor_l = NULL;
}



int SocketU::bind_s(Adress serv){
    if(!server)
        return 1;
    int rc = bind(sock, serv.addr(),*(serv.addrlen()));
    if(rc<0)
        return -1;
    else
        return 0;
}

ssize_t SocketU::operator<<(std::string s){
    static const char* str = s.c_str();
    Data dado;
    dado.ptr = (void*)str;
    dado.size = s.size();
    return *this<<dado;
}

ssize_t SocketU::operator<<(struct Data d){
    return sendto(sock, d.ptr, d.size, 0, receptor_, *receptor_l);
}

ssize_t SocketU::operator>>(std::string s){
    const char* str = new char[20];
    ssize_t qnt;
    Data dado;
    dado.ptr = (void*)str;
    dado.size = 20;
    qnt = *this>>dado;
    str = (char *) dado.ptr;
    s = string(str);
    delete str;
    return qnt;
}

ssize_t SocketU::operator>>(struct Data d){

    return recvfrom(sock, d.ptr, d.size, 0, emissor_, emissor_l);
}

struct sockaddr* SocketU::last(){
    return emissor_;
}
