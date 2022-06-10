#pastas
INCLUDE_FOLDER = ./include/
OBJ_FOLDER = ./obj/
#criação de arquivos
MAIN = main
SRC = main.cpp
OBJ = $(patsubst $(./)%.cpp, $(OBJ_FOLDER)%.o, $(SRC))

$(OBJ_FOLDER)%.o: main.cpp
	g++ -std=c++11 -g -Wall -c $< -o $@ -I$(INCLUDE_FOLDER)

all: $(OBJ)
	g++ -std=c++11 -g -Wall -o tp02 $(OBJ)

clean:
	@rm -rf $(OBJ_FOLDER)* tp02

	