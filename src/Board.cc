#include "Board.h"


Board::Board()
{
	
}

Board::~Board()
{
	for(long int i = pieceMasterlist.size() - 1; i >= 0; i--)
	{
		delete pieceMasterlist.at(i); 
		pieceMasterlist.erase(pieceMasterlist.begin() + i);
	}
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
				pieceLookupTable[x][y] = new Piece(Pos(x,y), 'R', (y != 0), this);
			}
			else if ((x == 1 || x == MAX_ROW_COL-2) && (y == MAX_ROW_COL-1 || y == 0))
			{
				// Knight
				pieceLookupTable[x][y] = new Piece(Pos(x,y), 'N', (y != 0), this);
			}
			else if ((x == 2 || x == MAX_ROW_COL-3) && (y == MAX_ROW_COL-1 || y == 0))
			{
				// Bishop
				pieceLookupTable[x][y] = new Piece(Pos(x,y), 'B', (y != 0), this);
			}
			else if ((x == 3) && (y == MAX_ROW_COL-1 || y == 0))
			{
				// Queen
				pieceLookupTable[x][y] = new Piece(Pos(x,y), 'Q', (y != 0), this);
			}
			else if ((x == 4) && (y == MAX_ROW_COL-1 || y == 0))
			{
				pieceLookupTable[x][y] = new Piece(Pos(x,y), 'K', (y != 0), this);
				// King
				if(y == 0)
					blackKing = pieceLookupTable[x][y];
				else
					whiteKing = pieceLookupTable[x][y];
			}
			else if (y == 1 || y == MAX_ROW_COL-2)
			{
				// Pawn
				pieceLookupTable[x][y] = new Piece(Pos(x,y), 'P', (y != 1), this);
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

Bitboard Board::getWhiteMask() const
{
	return whiteMask;
}

Bitboard Board::getBlackMask() const
{
	return blackMask;
}