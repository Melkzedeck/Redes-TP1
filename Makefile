
PROJ_SERVER=server

PROJ_CLIENT=client

PROJ_CLIENT2=client2

RM = rm -rf

# .c files
C_SOURCE=$(wildcard ./source/*.cpp)

# .h files
H_SOURCE=$(wildcard ./include/*.h)

# Object files
OBJ=$(subst .cpp,.o,$(subst source,objects,$(C_SOURCE)))

# Compiler
CC=g++

# Flags for compiler
CC_FLAGS=-W         \
         -Wall      \
         -pedantic

#
# Compilation and linking
#
all: objFolder $(PROJ_SERVER)

$(PROJ_SERVER): $(OBJ) $(PROJ_SERVER).o
	$(CC) -o $@ $^

#$(PROJ_CLIENT): $(OBJ) $(PROJ_CLIENT).o
#	$(CC) -o $@ $^
#
#$(PROJ_CLIENT2): $(OBJ) $(PROJ_CLIENT2).o
#	$(CC) -o $@ $^
#
./objects/%.o: ./source/%.cpp ./include/%.h
	$(CC) -c -o $@ $< $(CC_FLAGS)  

server.o: server.cpp $(H_SOURCE)
	$(CC) -c $< $(CC_FLAGS)

client.o: client.cpp $(H_SOURCE)
	$(CC) -c $< $(CC_FLAGS)

client2.o: client2.cpp $(H_SOURCE)
	$(CC) -c $< $(CC_FLAGS)


objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects
	@ $(RM) *.o
	@ $(RM) $(PROJ_SERVER) $(PROJ_CLIENT) $(PROJ_CLIENT2) *~