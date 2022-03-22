CC=g++
CFLAGS= -Wall -g
RM=rm -rf
CFORMAT=.cpp
EXE_FORMAT=.out

# diretórios
MAIN_DIR=./main
C_DIR=./Redes_components/src/
H_DIR=./Redes_components/include/
OBJ_DIR=./obj

# objetos e .cpp
C_DEPENDENCES=$(wildcard $(C_DIR)*$(CFORMAT))
OBJ_DEPENDENCES=$(subst $(CFORMAT),.o,$(subst $(C_DIR),$(OBJ_DIR)/, $(C_DEPENDENCES)))

MAIN_CPP=$(wildcard ./$(MAIN_DIR)/*$(CFORMAT))
MAIN_OUT=$(subst $(CFORMAT),$(EXE_FORMAT),$(subst $(MAIN_DIR)/,, $(MAIN_CPP)))


all: objs $(MAIN_OUT)
	@ echo


objs: make_dir_obj $(OBJ_DEPENDENCES)
	@ echo Objetos das classes gerados com sucesso
	@ echo

./%.out: $(MAIN_DIR)/%$(CFORMAT)
	@ echo compilando $@
	@ $(CC) $(CFLAGS) $< $(OBJ_DEPENDENCES) -o $@ -I$(H_DIR)
	@ echo 

$(OBJ_DIR)/%.o: $(C_DIR)%$(CFORMAT)
	@ echo Gerando $@ 
	@ $(CC) $(CFLAGS) -c $< -o $@ -I$(H_DIR)
	@ echo 

make_dir_obj:
	@ echo Gerando as dependencias do programa...
	@ [ -d "./obj" ] && echo "'./obj' already exist" || mkdir obj
	@ echo

cleanObj:
	@ rm $(OBJ_DIR)/*
	@ rmdir $(OBJ_DIR)
	@ echo "objetos excluídos"

clean: cleanObj
	@ rm  ./*.out
	@ echo "binários excluídos"