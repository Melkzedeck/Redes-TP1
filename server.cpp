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
using std::pair;
using std::make_pair;

void usage(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: %s <v4|v6> <server port>\n", argv[0]);
        printf("example: %s v4 51511\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

unsigned int nearby(const vector<pair<int, int>>& mundo, const pair<int, int>& cord){
	unsigned int distance=10000*2, pos=0;
	for(unsigned int i = 0; i<mundo.size(); i++){
		unsigned int dist;
		if((dist = abs(mundo[i].first-cord.first) + abs(mundo[i].second-cord.second))<distance){
			distance = dist;
			pos = i;
		}
	}
	return pos;
}

bool add(vector<pair<int, int>>& mundo, const pair<int, int>& cord){
	auto it = find(mundo.begin(), mundo.end(), cord);
	if(it==mundo.end()){
		mundo.push_back(cord);
		return true;
	} return false;
}

bool remove(vector<pair<int, int>>& mundo, const pair<int, int>& cord){
	auto it = find(mundo.begin(), mundo.end(), cord);
	if(it!=mundo.end()){
		mundo.erase(it);
		return true;
	} 
	return false;
}

void split(string& msg, string& command, string& position, int& x, int& y){
    
    string  cord, ord;
    command = msg.substr(0, msg.find_first_of(' '));
    position = msg.substr(msg.find_first_of(' ')+1);
    cord = position.substr(0, position.find_first_of(' '));
    ord = position.substr(position.find_first_of(' ')+1);
    cout << msg << endl << command << endl << position << endl << cord << endl << ord <<endl << command << endl;
    x = stoi(cord);
    y = stoi(ord);
}

int main(int argc, char **argv) {
    STclient::setTAM(500);
    vector<pair<int, int>> mundo;
    usage(argc, argv);
    Adress server(argv[1][1],argv[2]);
    Tserver sock(server);
    int num = 0;
    cout<< "bound to " << server.str()<<", waiting connections" << endl;
    STclient client = sock.waitConection();
    cout << "[log] connection from " << client.addr_str() << endl;
    string msg, pct, command, position;
    do{
        msg.clear();
        pct.clear();
        num = 0;
        do{
            num += client>>pct;
            if(num==0)
                break;
            msg += pct;
        }while(pct[pct.size()-1]!='\n');
        if(num==0)
            break;
        msg.pop_back();
        cout << msg << endl;
        if(msg == "list"){
            for(unsigned int i = 0; i < mundo.size(); i++){
                client << (to_string(mundo[i].first) + " " + to_string(mundo[i].second)+ " ");
            }
            client<<"\n";
            continue;
        }
        int x, y;
        split(msg, command, position, x, y);
        auto pos = make_pair(x,y);
        if(command == "add"){
            if(add(mundo, pos)){
                client<<(position+" added\n");
            }
            else{
                client<<(position+" already exist\n");
            }
        }
        else if(command == "rm"){
            if(remove(mundo, pos)){
                client<<(position+" removed\n");
            }
            else{
                client<<(position+" does not exist\n");
            }
        }
        else if(command == "query"){
            unsigned int i = nearby(mundo, pos);
            client << (to_string(mundo[i].first) + " " + to_string(mundo[i].second)+"\n");
        }
        
    } while(strcasecmp(msg.c_str(),"kill")!=0 && num!=0);
    cout << "[log] close connection of " << client.addr_str() << endl;
    return 0;
}