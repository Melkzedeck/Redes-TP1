#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <utility>
#include <math.h>

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

bool operator==(const local& lc1,const  local& lc2){
    return (lc1.x==lc2.x)&&(lc1.y==lc2.y);
}

bool operator!=(const local& lc1,const  local& lc2){
    return !(lc1==lc2);
}

//função usada na query: retorna o indice do lugar mais proximo
unsigned int nearby(const vector<local>& mundo, const local& cord);

bool add(vector<local>& mundo, const local& cord);

bool remove(vector<local>& mundo, const local& cord);

//divide o comando da posicao
char split(string& msg, string& position, int& x, int& y);

//execucao de comando: do início de uma string ate chegar em um '\n'
void comando(string& msg, string &resposta, vector<local>& mundo);



int main(int argc, char **argv) {
        usage_s(argc, argv);
    Tclient::setTamMax(500);//seta o maior numero de bytes de envio e de recebimento
    vector<local> mundo;
    Adress server(argv[1][1],argv[2]);
    Tserver sock(server);
    int num = 0;
    cout<< "bound to " << server.str()<<", waiting connections" << endl;
    string msg, resposta;
    do{ // permanece nesse loop enquanto o servidor não receber a mensagem "kill"
        Tclient client = sock.waitConection();//depois de conectado retorna uma classe com o descritor
        cout << "[log] connection from " << client.addr() << endl;
        string command, position, pct;
        do{ // permanece nesse loop enquanto o cliente estiver conectado com o servidor
            num = 0;
            num = client>>pct;
            msg += pct;
            if(num==0)
                break;
                        
            while(msg.find_first_of('\n')!=msg.npos){//permanece nesse loop enquanto a msg contiver um comando
                comando(msg,resposta,mundo);
                if(resposta!="kill")
                    client<<resposta;
                else
                    break;
            }
            
        } while(num!=0 && msg!="kill");
        cout << "[log] close connection of " << client.addr() << endl;
		client.Close();
    }while(msg != "kill");
    cout << "Kill the server" << endl;
    return 0;
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
    command = msg.substr(0, msg.find_first_of(' '));
    if(command != "add" && command != "rm" && command != "query"){
        x = 0;
        y = 0;
        position.clear();
    }
    else{
        position = msg.substr(msg.find_first_of(' ')+1);
        //dividindo a cordenada da ordenada 
        auto space = position.find_first_of(' ');
		try{
			x = stoi(position.substr(0, space));
			y = stoi(position.substr(space +1));
		} catch(std::invalid_argument& e){
			position = "NA";
		}
    }
    //abaixo lanca um char que sera recebido num switch
    if(strcasecmp(command.c_str(),"list")==0)
        return 'l';
    else if(strcasecmp(command.c_str(),"query")==0)
        return 'q';
    else if(strcasecmp(command.c_str(),"add")==0)
        return 'a';
    else if(strcasecmp(command.c_str(),"rm")==0)
        return 'r';
    else if(strcasecmp(command.c_str(),"kill")==0)
        return 'k';
    else
        return 'e';
}


void comando(string& msg, string &resposta, vector<local>& mundo){
    resposta.clear();
    size_t eom = msg.find_first_of('\n');//eom = end of message
    string position, command;
    command = msg.substr(0,eom);
    msg = msg.substr(eom+1);
    cout << command << endl;
    local pos;
	char aux = split(command, position, pos.x, pos.y);
	if(position == "NA"){
		resposta = "comando inválido\n";
		return;
	}
    switch (aux){
        case 'l': //list
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
        break;
    }
        case 'k' : msg = "kill"; break; //kill
        case 'a': // add
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
        case 'r' : // rm
    {
        if(remove(mundo, pos)){
            resposta = (position+" removed\n");
        }
        else{
            resposta = (position+" does not exist\n");
        }
        break;
    }
        case 'q' : // query
    {
        if(mundo.size()==0)
            resposta = "none\n";
        unsigned int i = nearby(mundo, pos);
        resposta = (to_string(mundo[i].x) + " " + to_string(mundo[i].y)+"\n");
        break;
    }
        default:
		{
			resposta = "comando invalido\n";
			break;
		}
    }

}
