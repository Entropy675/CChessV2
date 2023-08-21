#include "LinuxClient.h"

LinuxClient::LinuxClient() 
	: connectIP(CONNECT_IP), baseWriteHead(19), writeHead(baseWriteHead), logfile("log.txt"), logstring("")
{
	initNcurses();
	setcchar(&li, L"─", A_NORMAL, 0, NULL);
	setcchar(&ld, L"│", A_NORMAL, 0, NULL);

	if(largeBoard)
	{
		sqSize.set(6, 4);
		offset.set(3, 2); // from top left corner of square		
	}
	else
	{
		sqSize.set(4, 2); // SQUARE board, visually clearer
		offset.set(2, 1); 
	}
}

LinuxClient::~LinuxClient()
{
    close(clientSocket);
	cleanupNcurses();
}

void LinuxClient::initNcurses()
{
	setlocale(LC_ALL,""); // Set the locale to the user's preferred one
	initscr();
	cbreak();
	noecho();

	attron(A_BOLD);
}

void LinuxClient::cleanupNcurses()
{
	endwin();
}

int LinuxClient::startConnection()
{
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	
    if (clientSocket < 0) 
	{
        log("Failed to create socket.");
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT); 
	
    if (inet_pton(AF_INET, connectIP.c_str(), &(serverAddress.sin_addr)) <= 0) 
	{
        log("Invalid address.");
        return 1;
    }

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) 
	{
        log("Connection failed.");
        return 1;
    }
	
	log("Connected to the chess engine.");

    return 0;
}


void LinuxClient::refreshScreen()
{
	clear();
	
	drawBoard();
	drawPieces();
	drawPieceBar();
	
	writeHead = baseWriteHead;
	
	refresh();
	
	wclear(logwin);
	mvwprintw(logwin, 1, 2, "=== LOG ===\n  %s", logstring.c_str());
	box(logwin, 0, 0);
	wrefresh(logwin);
	
	moveToInputPos();
}

void LinuxClient::startGameLoop()
{
	using namespace std;
    // Exchange data with the chess engine through 'clientSocket' when connected
	while(true)
	{
		bool redraw = true;

		std::string uinp;
		userInput(uinp);

		//addstr(uinp.c_str());

		// for valid input, must be of format "c# c#", where c = char in "abcdefgh"
		// could also support p = kqrnbp in format "pc#", where only one valid move can go there.
		// (requires some assumptions about where the pieces are in the array & some validity checks)
		// (could also just scan through all pieces, there are only 16... its not really slow.)
		// positions, moving from p1 to p2
		Pos p1(0,0), p2(0,0);
		
		regex pattern("[a-h][1-8] [a-h][1-8]");
		if(regex_match(uinp, pattern))
		{
			p1.setX(uinp[0] - 'a'); // ['a', 'b', 'c'] -> [0, 1, 2]
			p1.setY(8 - (uinp[1] - '0')); // ['8', '7', '6'] -> [0, 1, 2]
		
			p2.setX(uinp[3] - 'a');
			p2.setY(8 - (uinp[4] - '0'));
		}
	
		regex pattern1("[a-h][1-8][a-h][1-8]");
		if(regex_match(uinp, pattern1))
		{
			p1.setX(uinp[0] - 'a'); // ['a', 'b', 'c'] -> [0, 1, 2]
			p1.setY(8 - (uinp[1] - '0')); // ['8', '7', '6'] -> [0, 1, 2]

			p2.setX(uinp[2] - 'a');
			p2.setY(8 - (uinp[3] - '0'));
		}
		
		regex pattern2("[kqrbnp][a-h][1-8]");
		if(regex_match(uinp, pattern2))
		{
			// this is a valid input for single location given (only one possible valid move)
			// TODO: check each valid move that leads to this square, if there are more then one then don't do anything
		}
		
		if(uinp == string("tg"))
			toggleSize();
		else if(uinp == string("connect"))
			startConnection();
		else if(uinp == string("exit"))
			break;
		
		//log.append("Attempt: " + p1.toString() + " " + p2.toString() + "\n");
		//log.append("game->movePiece(Pos" + p1.toString() + ", Pos" + p2.toString() + ");\n"); 
		
		// SEND THE MOVE TO THE SERVER
		// HANDLE MOVE UPDATES OR FEN STRING RESYNCS


		if(redraw)
			refreshScreen();
	}
}

void LinuxClient::toggleSize()
{
	largeBoard = !largeBoard;
	
	if(largeBoard)
	{
		sqSize.set(6, 4);
		offset.set(3, 2);  // from top left corner of square	
		logwinX += 16;
		logwinHeight += 15;
		wresize(logwin, logwinHeight, logwinWidth);
		mvwin(logwin, logwinY, logwinX);
	}
	else
	{

		sqSize.set(4, 2); // more tight fitting board
		offset.set(2, 1); 
		logwinX -= 16;
		logwinHeight -= 15;
		wresize(logwin, logwinHeight, logwinWidth);
		mvwin(logwin, logwinY, logwinX);
	}
	
	refreshScreen();
}

void LinuxClient::userInput(std::string& uinp)
{
	char ch; // TODO: add backspace functionality 
	while(true)
	{
		ch = getch();
		addch(ch);
		uinp += ch;
		refresh();
		if(ch == '\n')
			break;
	}
	
	uinp.erase(uinp.end()-1); // null terminates, removes \n from uinp
}

void LinuxClient::log(const char* sin)
{
	std::string s(sin);
	std::size_t pos = 0;
	while ((pos = s.find('\n', pos)) != std::string::npos) {
		s.replace(pos, 1, "\n  ");
		pos += 3; // move past the newly inserted characters
	}
	
	if(logfile.is_open())
	{
		logfile << s << std::endl;
	}
	
	logstring = s;
//	mvwprintw(logwin, 3, 2, "%s", s.c_str());
//	box(logwin, 0, 0);
//	wrefresh(logwin);
}


void LinuxClient::moveToInputPos() const
{
	move(sqSize.getY()*MAX_ROW_COL + 2, 0); // move to position for input
}

void LinuxClient::print(const std::string& s)
{
	printAt(writeHead++, 0, s);
}

void LinuxClient::printAt(int x, int y, const std::string& s) const
{
	mvprintw(x, y, "%s", s.c_str());
	moveToInputPos();
}

void LinuxClient::drawBoard()
{
	// draws an empty labeled board based off sqSize
	for(int x = 0; x <= MAX_ROW_COL; x++)
	{
		for(int line = 1; line < sqSize.getY()*MAX_ROW_COL; line++)
		{
			move(line, sqSize.getX()*x);
			add_wch(&ld); // ld is "─" wide character
		}
	}

	for(int y = 0; y <= MAX_ROW_COL; y++)
	{
		move(sqSize.getY()*y, 1);
		for(int line = 0; line < sqSize.getX()*MAX_ROW_COL - 1; line++)
			add_wch(&li); // li is "│" wide character
	}

	for(int y = 1; y <= MAX_ROW_COL -1; y++)
	{
		for(int x = 1; x <= MAX_ROW_COL -1; x++)
		{
			move(sqSize.getY()*y, sqSize.getX()*x);
			addstr("+");
		}
	}	
	
	move(sqSize.getY()*MAX_ROW_COL + 1, 0);
	
	const char chars[9] = "abcdefgh";
	
	for(int y = 0; y < MAX_ROW_COL; y++)
	{
		// works well with largeboard 
		for(int i = 0; i < sqSize.getX()/2; i++)
			addstr(" ");
			
		addch(chars[y]);
		
		for(int i = 0; i < sqSize.getX()/2 - 1; i++)
			addstr(" ");
	}
}

void LinuxClient::drawPieces()
{
	// draws all pieces in their right positions based off sqSize and offset
	for(int x = 0; x < MAX_ROW_COL; x++)
	{
		for(int y = 0; y < MAX_ROW_COL; y++)
		{
			Pos p(y, x);
			move(x*sqSize.getY() + offset.getY(), y*sqSize.getX() + offset.getX());
			
			cchar_t ctemp;
			wideChessConversion(game.getPiece(p), ctemp);
			add_wch(&ctemp);
		
		}
		
		int y1 = getcury(stdscr);
		move(y1, sqSize.getX()*MAX_ROW_COL + 2);
		addstr((std::to_string(MAX_ROW_COL - x)).c_str());
	}
}

void LinuxClient::drawPieceBar()
{
	// draw the dead pieces in 2 different lines for white and black
	move(sqSize.getY()*MAX_ROW_COL + 4, 0);
	cchar_t ctemp;
	
	const char* spaces = "        ";
	
	
	// TODO: refactor these 2 for loops to eliminate getWhitePieces/getBlackPieces if possible
	// or make more functions
	addstr(spaces);
	for(char c : game.getDeadWhitePieces())
	{
		wideChessConversion(c, ctemp);
		add_wch(&ctemp);
	}
	
	wideChessConversion(game.isWhiteTurn() ? 'k' : 'K', ctemp);
	addstr("\n");
	if(game.isCheckOnBoard())
	{
		if(game.isWhiteTurn())
			addstr("  WHITE ");
		else
			addstr("  BLACK ");
	}
	else 
		addstr(spaces);
	
	addstr("- --<<");
	add_wch(&ctemp);
	add_wch(&ctemp);
	add_wch(&ctemp);
	addstr(">>-- -");
	
	if(game.isCheckOnBoard())
		addstr(" CHECK  ");
	else 
		addstr(spaces);
	addstr("\n");
	
	addstr(spaces);
	for(char c : game.getDeadBlackPieces())
	{
		wideChessConversion(c, ctemp);
		add_wch(&ctemp);
	}
}

// this is where the convention lowercase = white is set
void LinuxClient::wideChessConversion(char c, cchar_t& cch)
{
	wchar_t tmp = ' ';
	if(c == 'r')
		tmp = L'♜';
	if(c == 'n')
		tmp = L'♞';
	if(c == 'b')
		tmp = L'♝';
	if(c == 'k')
		tmp = L'♚';
	if(c == 'q')
		tmp = L'♛';
	if(c == 'p')
		tmp = L'♟';
	if(c == 'R')
		tmp = L'♖';
	if(c == 'N')
		tmp = L'♘';
	if(c == 'B')
		tmp = L'♗';
	if(c == 'K')
		tmp = L'♔';
	if(c == 'Q')
		tmp = L'♕';
	if(c == 'P')
		tmp = L'♙';
	
	
	setcchar(&cch, &tmp, A_NORMAL, 0, NULL);
}
