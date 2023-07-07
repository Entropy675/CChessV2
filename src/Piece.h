#ifndef PIECE
#define PIECE

#include "Board.h"

class Piece
{
	public:
	Piece(Board* = nullptr);
	~Piece();
	
	private:
	Board* game;
	
};

#endif