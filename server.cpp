#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <utility>
#include "Adress.h"
#include "socket_TCP.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::find;
using std::to_string;

typedef struct local{
    int x;
    int y;
}local;

void usage(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: %s <v4|v6> <server port>\n", argv[0]);
        printf("example: %s v4 51511\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

bool operator==(const local& lc1,const  local& lc2){
    return (lc1.x==lc2.x)&&(lc1.y==lc2.y);
}

bool operator!=(const local& lc1,const  local& lc2){
    return !(lc1==lc2);
}

unsigned int nearby(const vector<local>& mundo, const local& cord){
	unsigned int distance=10000*2, pos=0;
	for(unsigned int i = 0; i<mundo.size(); i++){
		unsigned int dist;
		if((dist = abs(mundo[i].x-cord.x) + abs(mundo[i].y-cord.y))<distance){
			distance = dist;
			pos = i;
		}
	}
	return pos;
}

bool add(vector<local>& mundo, const local& cord){
	auto it = find(mundo.begin(), mundo.end(), cord);
	if(it==mundo.end()){
		mundo.push_back(cord);
		return true;
	} return false;
}

bool remove(vector<local>& mundo, const local& cord){
	auto it = find(mundo.begin(), mundo.end(), cord);
	if(it!=mundo.end()){
		mundo.erase(it);
		return true;
	} 
	return false;
}

void split(string& msg, string& command, string& position, int& x, int& y){
    
    string  cord, ord;
    //dividindo o comando da posicao
    command = msg.substr(0, msg.find_first_of(' '));
    if(command == "list" || command == "kill"){
        x = 0;
        y = 0;
        position.clear();
    }
    else{
        position = msg.substr(msg.find_first_of(' ')+1);
        //dividindo a cordenada da ordenada 
        cord = position.substr(0, position.find_first_of(' '));
        ord = position.substr(position.find_first_of(' ')+1);
        //transformando string em inteiro
        x = stoi(cord);
        y = stoi(ord);
    }
}


char function(const string& command){
    if(strcasecmp(command.c_str(),"list")==0){
        return 'l';
    }
    else if(strcasecmp(command.c_str(),"query")==0){
        return 'q';
    }
    else if(strcasecmp(command.c_str(),"add")==0){
        return 'a';
    }
    else if(strcasecmp(command.c_str(),"rm")==0){
        return 'r';
    }
    else{
        return 'e';
    }
}

int main(int argc, char **argv) {
    STclient::setTAM(500);
    vector<local> mundo;
    usage(argc, argv);
    Adress server(argv[1][1],argv[2]);
    Tserver sock(server);
    int num = 0;
    cout<< "bound to " << server.str()<<", waiting connections" << endl;
    string msg;
    do{
        STclient client = sock.waitConection();
        cout << "[log] connection from " << client.addr_str() << endl;
        string pct, command, position;
        do{
            msg.clear();
            pct.clear();
            num = 0;
            do{
                num += client>>pct;
                if(num==0)
                    break;
                msg += pct;
            }while(pct[pct.size()-1]!='\n'&& num>0);
            msg.pop_back();
            if(num==0 || strcasecmp(msg.c_str(),"kill")==0)
                break;
            cout << msg << endl;
            local pos;
            split(msg, command, position, pos.x, pos.y);
            if(pos.x>9999||pos.y>9999){
                client<<"out of range\n";
                continue;
            }
            switch (function(command)){
            case 'l':
            {
                msg.clear();
                for(unsigned int i = 0; i < mundo.size(); i++){
                    msg += (to_string(mundo[i].x) + " " + to_string(mundo[i].y)+ " ");
                    
                }
                msg += "\n";
                client<<msg;
                break;
            }
            case 'k' : break;
            case 'a':
            {
                //cout<<(int)*(command.c_str()) << endl;
                if(mundo.size()==50){
                    client<<"limit exceeded\n";
                }
                else if(add(mundo, pos)){
                    client<<(position+" added\n");
                }
                else{
                    client<<(position+" already exist\n");
                }
                break;
            }
            case 'r' :
            {
                if(remove(mundo, pos)){
                    client<<(position+" removed\n");
                }
                else{
                    client<<(position+" does not exist\n");
                }
                break;
            }
            case 'q' :
            {
                if(mundo.size()==0)
                    client<<"none\n";
                unsigned int i = nearby(mundo, pos);
                client << (to_string(mundo[i].x) + " " + to_string(mundo[i].y)+"\n");
                break;
            }
            default: break;
            }
        } while(num!=0 && msg!="kill");
        cout << "[log] close connection of " << client.addr_str() << endl;
    }while(strcasecmp(msg.c_str(),"kill")!=0);
    cout << "Kill the server" << endl;
    return 0;
}