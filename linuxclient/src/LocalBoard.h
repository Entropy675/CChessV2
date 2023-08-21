#ifndef LOCALBOARD_H
#define LOCALBOARD_H

#include <vector>
#include "Pos.h"
#include "defs.h"

class LocalBoard
{
	public:
	LocalBoard();
	~LocalBoard();
	
	void setStartingBoard(bool = true); // boolean for local color, true = white
	
	char& getPiece(Pos);
	void placePiece(Pos from, Pos to);
	
	const std::vector<char>& getDeadWhitePieces() const;
	const std::vector<char>& getDeadBlackPieces() const;
	
	bool isWhiteTurn();
	bool isCheckOnBoard();
	
	private:
	char board[MAX_ROW_COL][MAX_ROW_COL];
	
	std::vector<char> deadWhitePieces;
	std::vector<char> deadBlackPieces;

	bool enPassantActive;
	
	Pos* previousPieceMoved;
	
	bool whitePerspective; // for which way to display the board, display board flipped for black (nothing to do with gamelogic, only view)
	bool whiteTurn;
	
	int halfmoveCount;
	int turnCountFEN;
	int moveCount;
	
	bool checkmate;
	bool check;
};



#endif