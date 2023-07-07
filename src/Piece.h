#ifndef PIECE
#define PIECE

#include "Pos.h"
#include "Board.h"
#include "Bitboard.h"

class Piece
{
	public:
	Piece(Pos p, char t, bool, Board*);
	~Piece();
	
	Bitboard validMoves(Bitboard whiteMask = Bitboard(), Bitboard blackMask = Bitboard());
	Bitboard validCaptures(Bitboard whiteMask = Bitboard(), Bitboard blackMask = Bitboard());
	
	const Pos& getPos() const;
	char getCharacter() const;
	
	bool isWhite() const;
	bool isDead() const;
	bool hasMoved() const;
	
	private:
	Board* game;
	const bool white;
	
	char type; // {R, N, B, K, Q, P} -> {Rook, Knight, Bishop, King, Queen, Pawn}
	Pos pos;
	
	bool moved;
	bool dead;
	
	//std::vector<MoveBehaviour*> movebehavArr; // delegate isValidMove & validMoves to the sum of all these
};

#endif