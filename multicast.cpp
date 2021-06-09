#include "multicast.h"

void multicast::new_user(const int& sock){
    users.insert(std::pair<int, std::set<std::string>>(sock,std::set<std::string>()));
}


void multicast::subscribe(const int& sock, const std::string& tag){
    std::map<std::string, std::set<int>>::iterator it;
    std::map<int,std::set<std::string>>::iterator it1;
    it = tags.find(tag);
    if(it!=tags.end()){
        it->second.insert(sock);
    }
    else
        tags.insert(std::pair<std::string, std::set<int>>(tag, std::set<int>({sock})));
    it1 = users.find(sock);
    it1->second.insert(tag);
}

bool multicast::unsubscribe(const int& sock, const std::string& tag){
    std::map<std::string, std::set<int>>::iterator it;
    std::map<int,std::set<std::string>>::iterator it1;
    it = tags.find(tag);
    it1 = users.find(sock);
    if(it!=tags.end() && it1!=users.end()){
        it->second.erase(sock);
        it1->second.erase(tag);
        return true;
    }
    else if(it==tags.end() && it1==users.end())
        return false;
    else
        throw std::runtime_error("erro no conjunto de tags ou de users");
    
}

ssize_t multicast::msg(const std::string& msg, const std::string& tag){
    std::map<std::string, std::set<int>>::iterator it;
    std::set<int>::iterator it1;
    it = tags.find(tag);
    for(it1 = it->second.begin(); it1 != it->second.end(); it1++){
        send(*it1, msg.c_str(), msg.length(), 0);
    }
}
