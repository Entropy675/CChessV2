#include "LocalBoard.h"


LocalBoard::LocalBoard() : whiteTurn(true)
{
	
}

LocalBoard::~LocalBoard()
{
	
}

char& LocalBoard::getPiece(Pos p)
{
	return board[p.getX()][p.getY()];
}
const std::vector<char>& LocalBoard::getDeadWhitePieces() const
{
	return deadWhitePieces;
}

const std::vector<char>& LocalBoard::getDeadBlackPieces() const
{
	return deadBlackPieces;
}

bool LocalBoard::isWhiteTurn()
{
	return whiteTurn;
}

bool LocalBoard::isCheckOnBoard()
{
	return check;
}