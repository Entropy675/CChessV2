obj = obj/
src = src/

OBJS = $(obj)GameClient.o $(obj)main.o

TARGET = ../bin/Windows/CChess.exe
COMPILERFLAG = -Wl,-subsystem,windows -ISDL_include/
LIB = -L. -lmingw32 -lSDL2main -lSDL2 -lws2_32 -luser32 -static-libstdc++ -static-libgcc 
CC = g++

ICON_WINRESOURCE = ../icon/icon.res


$(TARGET): $(OBJS) $(ICON_WINRESOURCE)
	$(CC) $(COMPILERFLAG) $(INCLUDE) $(OBJS) $(LIB) $(ICON_WINRESOURCE) -o $(TARGET)

$(ICON_WINRESOURCE): ../icon/icon.rc
	windres $< -O coff -o $@

$(obj)GameClient.o: $(src)GameClient.cc $(src)GameClient.h $(src)defs.h
	$(CC) $(COMPILERFLAG) $(INCLUDE) -c $(src)GameClient.cc -o $(obj)GameClient.o 
	
$(obj)main.o: $(src)main.cc $(src)defs.h
	$(CC) $(COMPILERFLAG) $(INCLUDE) -c $(src)main.cc -o $(obj)main.o

	
clean:
	find . -type f -name '*.o' -delete
	rm $(TARGET)