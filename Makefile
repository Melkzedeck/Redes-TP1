CC_FLAGS=-W         \
         -Wall      \
         -pedantic  

all: exe

exe: client server client2
	rm server.o
	rm client.o
	rm client2.o
	rm Adress.o
	rm socket_TCP.o

server:	Adress.o socket_TCP.o
	g++ $(CC_FLAGS) -c server.cpp 
	g++ -o server server.o Adress.o socket_TCP.o

client: Adress.o socket_TCP.o
	g++ $(CC_FLAGS) -c client.cpp 
	g++ -o client client.o Adress.o socket_TCP.o

client2: Adress.o socket_TCP.o
	g++ $(CC_FLAGS) -c client2.cpp 
	g++ -o client2 client2.o Adress.o socket_TCP.o

Adress.o: Adress.h
	g++ $(CC_FLAGS) -c Adress.cpp 

socket_TCP.o: socket_TCP.h
	g++ $(CC_FLAGS) -c socket_TCP.cpp 

clean:
	rm server client client2
	rm test_0.out test_1.out test_2.out test_3.out

