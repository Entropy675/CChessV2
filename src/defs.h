#ifndef DEFS_H
#define DEFS_H

#define C_FAIL			0
#define C_PASS			1
#define MAX_ROW_COL		8
#define PORT 			50123

#define ENGINE_DELAY_MS		10
#define SOCKET_TIMEOUT_PING	100
#define RESEND_CMD_PING		10
#define RECIEVED_DATA_BUFF	1024

enum class ChessStatus 
{
	FAIL,
	SUCCESS,
	PROMOTE
};

constexpr int MAX_CLIENTS = 5;  // Maximum number of clients

class Board;
class Piece;

#endif