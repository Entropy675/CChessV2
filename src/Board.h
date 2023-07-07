#ifndef BOARD
#define BOARD

#include <vector>

#include "Pos.h"
#include "Bitboard.h"
#include "Piece.h"

class Board
{
	public:
	
	Board();
	~Board();
	
	ChessStatus movePiece(Pos a, Pos b);
	
	private:
	void setStartingBoard();
	
	// ONLY USE FOR CONSTANT TIME LOOKUP, NOT MOVE GEN
	Piece* pieceLookupTable[MAX_ROW_COL][MAX_ROW_COL];
	
	std::vector<Piece*> pieceMasterlist;
	
	std::vector<Piece*> whitePieces;
	std::vector<Piece*> blackPieces;
	Bitboard whiteMask;
	Bitboard blackMask;
	Piece* blackKing;
	Piece* whiteKing;
};

#endif