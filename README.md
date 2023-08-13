# CChessV2

Version 2 of CChess Engine.

**!! Work in Progress !!** Everything in this README may or may not be completely functioning at any point. **!! Work in Progress !!**

This version of the CChess Engine uses sockets for greater inter-platform compatibility. The engine hosts a socket server that can be connected to by any client. The server is cross-platform and uses a strategy pattern to select and load OS-specific socket code. There are separate client programs for both Windows and Linux.

## Installation

### Windows:
- The executable requires the SDL2 graphics library, but all dlls/headers are included. No installation is required.
- Requires the Mingw64 toolchain.
- Run `make` in the root directory.

### Linux:
- Requires the Ncurses graphics library, terminal only application.
- Install Ncurses using your package manager.
- Ensure that your terminal font is monospace.
- Run `make` in the root directory.

## Build and Execution

When you execute any of the Makefiles in the root of the project, depending on your operating system, either the `/linuxclient` or `/windowsclient` folders will be built. Your client executable will be found in the subfolder for your operating system. The executable generated at the root of the project is the server.

To use the chess engine:

1. Build the program at the root (it will also call the Makefile in the corresponding OS client folder).
2. Run the chess engine server (`CChess`) generated at the root.
3. Connect to the chess engine with a client (in either `/linuxclient` or `/windowsclient`).

By default, the port used for communication is 50123, as set in the `defs.h` file in the `/src` folder of any executable. The port is determined at compile time and can be set in the `defs.h` before building if you want a unique one for your server/clients (which can be a bit annoying **WIP**).
** the default IP is currently automatically set to local host, will soon be allowing you to change that... **

