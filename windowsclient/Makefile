obj = obj/
src = src/

OBJS = $(obj)GameClient.o $(obj)main.o

TARGET = CChessClient.exe
COMPILERFLAG = -Wl,-subsystem,windows -ISDL_include/
LIB = -L. -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -luser32 -static-libstdc++ -static-libgcc 
CC = g++


$(TARGET): $(OBJS)
	$(CC) $(COMPILERFLAG) $(INCLUDE) $(LIB) $(OBJS)  -o $(TARGET)

$(obj)GameClient.o: $(src)GameClient.cc $(src)GameClient.h
	$(CC) $(COMPILERFLAG) $(INCLUDE) -c $(src)GameClient.cc -o $(obj)GameClient.o 
	
$(obj)main.o: $(src)main.cc
	$(CC) $(COMPILERFLAG) $(INCLUDE) -c $(src)main.cc -o $(obj)main.o

	
clean:
	find . -type f -name '*.o' -delete
	rm $(TARGET)