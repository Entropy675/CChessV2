obj = obj/
src = src/

OBJS = $(obj)GameClient.o $(obj)main.o $(obj)Socket.o $(obj)Text.o $(obj)Window.o

TARGET = ../bin/Windows/CChess.exe
COMPILERFLAG = -Wl,-subsystem,windows -ISDL_include/
LIB = -L. -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lws2_32 -luser32 -static-libstdc++ -static-libgcc 
CC = g++

ICON_WINRESOURCE = ../icon/icon.res


$(TARGET): $(OBJS) $(ICON_WINRESOURCE)
	$(CC) $(COMPILERFLAG) $(OBJS) $(LIB) $(ICON_WINRESOURCE) -o $(TARGET)

$(ICON_WINRESOURCE): ../icon/icon.rc
	windres $< -O coff -o $@

$(obj)GameClient.o: $(src)GameClient.cc $(src)GameClient.h $(src)Socket.h $(src)Text.h $(src)defs.h $(src)Window.h
	$(CC) $(COMPILERFLAG) -c $(src)GameClient.cc -o $(obj)GameClient.o 
	
$(obj)main.o: $(src)main.cc $(src)defs.h
	$(CC) $(COMPILERFLAG) -c $(src)main.cc -o $(obj)main.o

$(obj)Socket.o: $(src)Socket.cc $(src)Socket.h $(src)defs.h
	$(CC) $(COMPILERFLAG) -c $(src)Socket.cc -o $(obj)Socket.o
	
$(obj)Text.o: $(src)Text.cc $(src)Text.h $(src)defs.h $(src)Window.h
	$(CC) $(COMPILERFLAG) -c $(src)Text.cc -o $(obj)Text.o

$(obj)Window.o: $(src)Window.cc $(src)Window.h $(src)defs.h
	$(CC) $(COMPILERFLAG) -c $(src)Window.cc -o $(obj)Window.o
	
clean:
	find . -type f -name '*.o' -delete
	rm $(TARGET)