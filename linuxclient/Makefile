PATH2SRC = src/
PATH2OBJ = obj/

CC = g++ -Wall -g

ENGINE_SOURCES = $(PATH2OBJ)LinuxClient.o

LinuxClient: $(ENGINE_SOURCES) $(PATH2OBJ)main.o
	$(CC) -o LinuxClient $(ENGINE_SOURCES) $(PATH2OBJ)main.o

$(PATH2OBJ)LinuxClient.o: $(PATH2SRC)LinuxClient.cc $(PATH2SRC)LinuxClient.h  $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)LinuxClient.cc -o $(PATH2OBJ)LinuxClient.o

$(PATH2OBJ)main.o: $(PATH2SRC)main.cc $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)main.cc -o $(PATH2OBJ)main.o
	
.PHONY: clean
clean:
		find . -type f -name '*.o' -delete
		rm -f LinuxClient