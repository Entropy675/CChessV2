#include "Board.h"


Board::Board()
{
	
}

Board::~Board()
{
	
}

ChessStatus Board::movePiece(Pos a, Pos b)
{
	
	return ChessStatus::FAIL;
}

void Board::setStartingBoard()
{
	// should be empty for the first call, for every other call the following two loops run and reset the vecs.
	for(long int i = pieceMasterlist.size() - 1; i >= 0; i--)
	{
		delete pieceMasterlist.at(i); 
		pieceMasterlist.erase(pieceMasterlist.begin() + i);
	}

	for(int x = 0; x < MAX_ROW_COL; x++)
	{
		for(int y = 0; y < MAX_ROW_COL; y++)
		{
			if((x == 0 || x == MAX_ROW_COL-1) && (y == MAX_ROW_COL-1 || y == 0))
			{
				// Rook
			}
			else if ((x == 1 || x == MAX_ROW_COL-2) && (y == MAX_ROW_COL-1 || y == 0))
			{
				// Knight
			}
			else if ((x == 2 || x == MAX_ROW_COL-3) && (y == MAX_ROW_COL-1 || y == 0))
			{
				// Bishop
			}
			else if ((x == 3) && (y == MAX_ROW_COL-1 || y == 0))
			{
				// Queen
			}
			else if ((x == 4) && (y == MAX_ROW_COL-1 || y == 0))
			{
				// King
				//if(y == 0)
				//	blackKing = whatever you make;
				//if(y == MAX_ROW_COL-1)
				//	whiteKing = whatever you make;
			}
			else if (y == 1 || y == MAX_ROW_COL-2)
			{
				// Pawn
			}

			if(y == 0)
			{
				blackPieces.push_back(pieceLookupTable[x][y]);
			}
			else if(y == MAX_ROW_COL-1)
			{
				whitePieces.push_back(pieceLookupTable[x][y]);
			}
			
		}
	}
	
	// add all the pawns after, so they are all together at the end...
	for(int i = 0; i < MAX_ROW_COL; i++)
	{
		blackPieces.push_back(pieceLookupTable[i][1]);
		whitePieces.push_back(pieceLookupTable[i][MAX_ROW_COL-2]);
	}
}