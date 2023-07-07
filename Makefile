PATH2SRC = src/
PATH2OBJ = obj/

CC = g++ -Wall -g

ENGINE_SOURCES = $(PATH2OBJ)ChessEngine.o $(PATH2OBJ)Pos.o $(PATH2OBJ)Board.o $(PATH2OBJ)Piece.o $(PATH2OBJ)Bitboard.o

CChess: $(ENGINE_SOURCES) $(PATH2OBJ)main.o
	$(CC) -o CChess $(ENGINE_SOURCES) $(PATH2OBJ)main.o
	$(MAKE) -C linuxclient

$(PATH2OBJ)ChessEngine.o: $(PATH2SRC)ChessEngine.cc $(PATH2SRC)ChessEngine.h  $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)ChessEngine.cc -o $(PATH2OBJ)ChessEngine.o

$(PATH2OBJ)main.o: $(PATH2SRC)main.cc $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)main.cc -o $(PATH2OBJ)main.o
	
$(PATH2OBJ)Pos.o: $(PATH2SRC)Pos.cc $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)Pos.cc -o $(PATH2OBJ)Pos.o
	
$(PATH2OBJ)Board.o: $(PATH2SRC)Board.cc $(PATH2SRC)Piece.h $(PATH2SRC)Pos.h $(PATH2SRC)Bitboard.h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)Board.cc -o $(PATH2OBJ)Board.o
	
$(PATH2OBJ)Piece.o: $(PATH2SRC)Piece.cc $(PATH2SRC)Pos.h $(PATH2SRC)Bitboard.h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)Piece.cc -o $(PATH2OBJ)Piece.o
	
$(PATH2OBJ)Bitboard.o: $(PATH2SRC)Bitboard.cc $(PATH2SRC)Pos.h $(PATH2SRC)defs.h
	$(CC) -c $(PATH2SRC)Bitboard.cc -o $(PATH2OBJ)Bitboard.o
	
.PHONY: clean
clean:
		find . -type f -name '*.o' -delete
		rm -f CChess
		rm -f linuxclient/LinuxClient