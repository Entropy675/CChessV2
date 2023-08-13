#ifndef DEFS_H
#define DEFS_H

#define C_FAIL			0
#define C_PASS			1
#define MAX_ROW_COL		8

enum class ChessStatus 
{
	FAIL,
	SUCCESS,
	PROMOTE
};

constexpr int PORT = 50123;
constexpr int MAX_CLIENTS = 5;  // Maximum number of clients

class Board;
class Piece;

#endif