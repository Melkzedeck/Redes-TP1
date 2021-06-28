
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
all: objFolder $(PROJ_SERVER) $(PROJ_CLIENT) $(PROJ_CLIENT2)

$(PROJ_SERVER): ./obj/$(PROJ_SERVER).o $(OBJ)
	$(CC) -o $@ $^

$(PROJ_CLIENT): ./obj/$(PROJ_CLIENT).o $(OBJ)
	$(CC) -o $@ $^

$(PROJ_CLIENT2): ./obj/$(PROJ_CLIENT2).o $(OBJ)
	$(CC) -o $@ $^

./objects/%.o: ./source/%.cpp ./include/%.h
	@ $(CC) -c -o $@ $< $(CC_FLAGS)  

./obj/%.o: %.cpp $(H_SOURCE)
	@ $(CC) -c -o $@ $< $(CC_FLAGS)  

objFolder:
	@ mkdir -p objects
	@ mkdir -p obj

clean:
	@ $(RM) ./objects/*.o ./obj/*.o *~
	@ rmdir objects obj
	@ $(RM) $(PROJ_SERVER) $(PROJ_CLIENT) $(PROJ_CLIENT2) *~
	@ echo 'Objetos e programas removidos'