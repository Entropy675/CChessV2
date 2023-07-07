#include "Piece.h"

Piece::Piece(Pos p, char t, bool w, Board* g) : game(g), white(w), type(t), pos(p)
{
	
}

Piece::~Piece()
{
	
}

Bitboard Piece::validMoves(Bitboard whiteMask, Bitboard blackMask)
{
	Bitboard b; // is empty, but whiteMask and blackMask should at minimum have 1 value, for the opponent king. (default to games masks)
	if(whiteMask == b)
		whiteMask = game->getWhiteMask();
	if(blackMask == b)
		blackMask = game->getBlackMask();
	
	return b;
}

Bitboard Piece::validCaptures(Bitboard whiteMask, Bitboard blackMask)
{
	Bitboard b; 
	if(whiteMask == b)
		whiteMask = game->getWhiteMask();
	if(blackMask == b)
		blackMask = game->getBlackMask();
	
	return b;
}
	
const Pos& Piece::getPos() const
{
	return pos;
}

char Piece::getCharacter() const
{
	return type;
}

bool Piece::isDead() const
{
	return dead;
}
	
bool Piece::isWhite() const
{
	return white;
}

bool Piece::hasMoved() const
{
	return moved;
}
