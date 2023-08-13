PATH2SRC = src/
PATH2OBJ = obj/

CC = g++ -Wall -g
LIB = -lncursesw

ENGINE_SOURCES = $(PATH2OBJ)LinuxClient.o $(PATH2OBJ)LocalBoard.o $(PATH2OBJ)Pos.o
TARGET = CChess

LinuxClient: $(ENGINE_SOURCES) $(PATH2OBJ)main.o
	$(CC) -o $(TARGET) $(ENGINE_SOURCES) $(PATH2OBJ)main.o $(LIB)

$(PATH2OBJ)LinuxClient.o: $(PATH2SRC)LinuxClient.cc $(PATH2SRC)LinuxClient.h $(PATH2SRC)LocalBoard.h $(PATH2SRC)Pos.h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)LinuxClient.cc -o $(PATH2OBJ)LinuxClient.o

$(PATH2OBJ)LocalBoard.o: $(PATH2SRC)LocalBoard.cc $(PATH2SRC)LocalBoard.h $(PATH2SRC)Pos.h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)LocalBoard.cc -o $(PATH2OBJ)LocalBoard.o

$(PATH2OBJ)Pos.o: $(PATH2SRC)Pos.cc $(PATH2SRC)Pos.h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)Pos.cc -o $(PATH2OBJ)Pos.o

$(PATH2OBJ)main.o: $(PATH2SRC)main.cc $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)main.cc -o $(PATH2OBJ)main.o
	
.PHONY: clean
clean:
		find . -type f -name '*.o' -delete
		rm -f $(TARGET)