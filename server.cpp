#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <utility>
#include <math.h>
#include "./include/Adress.h"
#include "./include/socket_TCP.h"

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
	unsigned int pos=0;
    double distance=10000*2, raio;
	for(unsigned int i = 0; i<mundo.size(); i++){
		double distx_2, disty_2;
        distx_2 = pow((mundo[i].x-cord.x),2);
        disty_2 = pow((mundo[i].y-cord.y),2);
        raio = sqrt(distx_2 + disty_2);
		if(raio<distance){
			distance = raio;
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

char split(string& msg, string& position, int& x, int& y){
    string command;
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
        auto space = position.find_first_of(' ');
        x = stoi(position.substr(0, space));
        y = stoi(position.substr(space +1));
    }
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
    else if(strcasecmp(command.c_str(),"kill")==0){
        return 'k';
    }
    else{
        return 'e';
    }
}



void comando(string& msg, string &resposta, vector<local>& mundo){
    resposta.clear();
    size_t eom = msg.find_first_of('\n');
    string position, command;
    command = msg.substr(0,eom);
    msg = msg.substr(eom+1);
    cout << command << '\t' << msg << endl;
    local pos;
    switch (split(command, position, pos.x, pos.y)){
        case 'l':
    {
        if(mundo.size()==0)
            resposta = "none ";
        else{
            for(unsigned int i = 0; i < mundo.size(); i++){
                resposta += (to_string(mundo[i].x) + " " + to_string(mundo[i].y) + " ");
            }   
        }
        resposta.pop_back();
        resposta += "\n";
        cout << resposta;
        break;
    }
        case 'k' : msg = "kill"; break;
        case 'a':
    {
        if(mundo.size()==50){
            resposta = "limit exceeded\n";
        }
        else if(add(mundo, pos)){
            resposta=(position+" added\n");
        }
        else{
            resposta = (position+" already exists\n");
        }
        break;
    }
        case 'r' :
    {
        if(remove(mundo, pos)){
            resposta = (position+" removed\n");
        }
        else{
            resposta = (position+" does not exist\n");
        }
        break;
    }
        case 'q' :
    {
        if(mundo.size()==0)
            resposta = "none\n";
        unsigned int i = nearby(mundo, pos);
        resposta = (to_string(mundo[i].x) + " " + to_string(mundo[i].y)+"\n");
        break;
    }
        default: break;
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
    string msg, resposta;
    do{
        STclient client = sock.waitConection();
        cout << "[log] connection from " << client.addr_str() << endl;
        string command, position, pct;
        do{
            num = 0;
            num = client>>pct;
            msg += pct;
            if(num==0)
                break;
                        
            while(msg.find_first_of('\n')!=msg.npos){
                comando(msg,resposta,mundo);
                if(resposta!="kill")
                    client<<resposta;
                else
                    break;
            }
            
        } while(num!=0 && msg!="kill");
        cout << "[log] close connection of " << client.addr_str() << endl;
    }while(msg != "kill");
    cout << "Kill the server" << endl;
    return 0;
}