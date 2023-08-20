PATH2SRC = src/
PATH2OBJ = obj/

# assuming MinGW toolchain for compiling on windows, assuming ncurses on linux
ifeq ($(OS),Windows_NT)
    TARGET = windowsclient
	BINLOCATION = bin/Windows/
	ENDTAG = .exe
	SOCKET = WindowsSocket
	LIB = -L. -lmingw32 -lws2_32 
	CC = g++
	ICON_WINRESOURCE = icon/icon.res
else ifeq ($(shell uname),Linux)
    TARGET = linuxclient
	BINLOCATION = bin/Linux/
	SOCKET = LinuxSocket
	CC = g++ -Wall -g
else
    $(error Unsupported operating system: $(OS))
endif

BINARY = $(BINLOCATION)CChessServer$(ENDTAG)

ENGINE_SOURCES = $(PATH2OBJ)main.o $(PATH2OBJ)ChessEngine.o $(PATH2OBJ)Pos.o $(PATH2OBJ)Board.o $(PATH2OBJ)Piece.o $(PATH2OBJ)Bitboard.o $(PATH2OBJ)$(SOCKET).o $(PATH2OBJ)SocketHandler.o

$(BINARY): $(ENGINE_SOURCES) $(ICON_WINRESOURCE)
	$(CC) $(ENGINE_SOURCES) $(ICON_WINRESOURCE) $(LIB) -o $(BINARY)
	$(MAKE) -C $(TARGET)

$(ICON_WINRESOURCE): icon/icon.rc
	windres $< -O coff -o $@

$(PATH2OBJ)ChessEngine.o: $(PATH2SRC)ChessEngine.cc $(PATH2SRC)ChessEngine.h $(PATH2SRC)SocketHandler.h $(PATH2SRC)$(SOCKET).h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)ChessEngine.cc -o $(PATH2OBJ)ChessEngine.o

$(PATH2OBJ)$(SOCKET).o: $(PATH2SRC)$(SOCKET).cc $(PATH2SRC)$(SOCKET).h $(PATH2SRC)SocketHandler.h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)$(SOCKET).cc -o $(PATH2OBJ)$(SOCKET).o
	
$(PATH2OBJ)SocketHandler.o: $(PATH2SRC)SocketHandler.cc $(PATH2SRC)SocketHandler.h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)SocketHandler.cc -o $(PATH2OBJ)SocketHandler.o
	
$(PATH2OBJ)Board.o: $(PATH2SRC)Board.cc $(PATH2SRC)Board.h $(PATH2SRC)Piece.h $(PATH2SRC)Bitboard.h $(PATH2SRC)Pos.h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)Board.cc -o $(PATH2OBJ)Board.o
	
$(PATH2OBJ)Piece.o: $(PATH2SRC)Piece.cc $(PATH2SRC)Piece.h $(PATH2SRC)Bitboard.h $(PATH2SRC)Pos.h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)Piece.cc -o $(PATH2OBJ)Piece.o
	
$(PATH2OBJ)Bitboard.o: $(PATH2SRC)Bitboard.cc $(PATH2SRC)Bitboard.h $(PATH2SRC)Pos.h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)Bitboard.cc -o $(PATH2OBJ)Bitboard.o

$(PATH2OBJ)Pos.o: $(PATH2SRC)Pos.cc $(PATH2SRC)Pos.h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)Pos.cc -o $(PATH2OBJ)Pos.o
	
$(PATH2OBJ)main.o: $(PATH2SRC)main.cc $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)main.cc -o $(PATH2OBJ)main.o
	
.PHONY: clean
clean:
		find . -type f -name '*.o' -delete
		rm -f icon/icon.res
		rm -f $(BINARY)
		rm -f $(BINLOCATION)CChess$(ENDTAG)
	
.PHONY: cleanobjects
cleanobjects:
		find . -type f -name '*.o' -delete
		rm -f icon/icon.res