#pragma once
#include "socket_TCP.h"
#include "Adress.h"
#include <map>
#include <set>
#include <stdexcept>

class multicast{
    //referrencia direta: user->tags inscritas
    std::map<int,std::set<std::string>> users;
    //referencia inversa: tag -> conj de usuarios
    std::map<std::string, std::set<int>> tags; 
public:
    multicast(){};
    void login(const int&);
    void logout(const int&);
    void subscribe(const int&,const std::string&);
    bool unsubscribe(const int&,const std::string&);
    bool is_subscriber(const int&,const std::string&);
    ssize_t msg(const std::string& msg, const std::string& tag);
};