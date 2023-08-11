obj = obj/
src = src/

OBJS = $(obj)Game.o $(obj)main.o

TARGET = CChessClient.exe
COMPILERFLAG = -Wl,-subsystem,windows
LIB = -L. -lmingw32 -lSDL2main -lSDL2 -luser32 -static-libstdc++ -static-libgcc
CC = g++
	
$(TARGET): $(OBJS)
	$(CC) $(COMPILERFLAG) $(LIB) $(OBJS)  -o $(TARGET)

$(obj)Game.o: $(src)Game.cc $(src)Game.h
	$(CC) -c $(src)Game.cc -o $(obj)Game.o
	
$(obj)main.o: $(src)main.cc
	$(CC) -c $(src)main.cc -o $(obj)main.o

	
clean:
	find . -type f -name '*.o' -delete
	rm $(TARGET)