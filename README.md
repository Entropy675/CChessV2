# CChessV2

Version 2 of CChess Engine.
Uses sockets for greater inter-platform compatibility. The engine hosts a socket server that is then connected to by any client.

The windows client is made with Simple DirectMedia Layer (SDL).
The linux client is using ncurses.
The server is currently coupled with some ncurses code, will be decoupled and is fully cross platform otherwise.

Developer Instructions:
- LINUX:
- Make the program at root (it will also call the makefile in the linuxclient file).
- Run the chess engine (CChess) generated at the root.
- Connect to the chess engine with a client (go to linux client folder, and run the LinuxClient)
- WINDOWS:
- You cannot currently run a server on windows by default.
- If you want to run a server, workaround currently is using WSL.
- For the client, set up and use a mingw64 terminal/toolchain, go to windowsclient folder, run make.
- There should be a CChessClient.exe compiled and built. The dependencies are included. 
