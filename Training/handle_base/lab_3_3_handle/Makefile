CC = gcc
CFLAGS = -Wall -g
LFLAGS = -lpthread
INC = ./inc
SRC = ./src
OBJ = ./obj
object = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(wildcard $(SRC)/*.c))
output = handle

all: $(object)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(output) $^

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -I $(INC) -o $@ -c $<
	
clean:
	rm -rf $(OBJ)/* $(output)
	
.PHONY: all clean
