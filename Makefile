PATH2SRC = src/
PATH2OBJ = obj/

CC = g++ -Wall -g

ENGINE_SOURCES = $(PATH2OBJ)ChessEngine.o

CChess: $(ENGINE_SOURCES) $(PATH2OBJ)main.o
	$(CC) -o CChess $(ENGINE_SOURCES) $(PATH2OBJ)main.o
	$(MAKE) -C linuxclient

$(PATH2OBJ)ChessEngine.o: $(PATH2SRC)ChessEngine.cc $(PATH2SRC)ChessEngine.h  $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)ChessEngine.cc -o $(PATH2OBJ)ChessEngine.o

$(PATH2OBJ)main.o: $(PATH2SRC)main.cc $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)main.cc -o $(PATH2OBJ)main.o
	
.PHONY: clean
clean:
		find . -type f -name '*.o' -delete
		rm -f CChess
		rm -f linuxclient/LinuxClient