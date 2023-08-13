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
	
	char& getPiece(Pos);
	const std::vector<char>& getDeadWhitePieces() const;
	const std::vector<char>& getDeadBlackPieces() const;
	
	bool isWhiteTurn();
	bool isCheckOnBoard();
	
	private:
	char board[MAX_ROW_COL][MAX_ROW_COL];
	std::vector<char> deadWhitePieces;
	std::vector<char> deadBlackPieces;
	
	bool whiteTurn;
	bool check;
	
};



#endif