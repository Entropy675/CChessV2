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
	
	private:
	void setStartingBoard();
	
	std::vector<Piece*> pieceMasterlist;
	
};

#endif