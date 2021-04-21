CC_FLAGS=-W         \
         -Wall      \
         -pedantic

all: exe

exe: client server server_mt
	rm server_mt.o
	rm server.o
	rm client.o
	rm Adress.o
	rm socket_TCP.o

server_mt:	Adress.o socket_TCP.o
	g++ $(CC_FLAGS) -c server_mt.cpp 
	g++ -o server_mt server_mt.o Adress.o socket_TCP.o

server:	Adress.o socket_TCP.o
	g++ $(CC_FLAGS) -c server.cpp 
	g++ -o server server.o Adress.o socket_TCP.o

client: Adress.o socket_TCP.o
	g++ $(CC_FLAGS) -c client.cpp 
	g++ -o client client.o Adress.o socket_TCP.o

Adress.o: Adress.h
	g++ $(CC_FLAGS) -c Adress.cpp 

socket_TCP.o: socket_TCP.h
	g++ $(CC_FLAGS) -c socket_TCP.cpp 

clean:
	rm server client server_mt
