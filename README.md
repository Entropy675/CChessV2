# CChessV2

Version 2 of CChess Engine.

**!! Work in Progress !!** Everything in this README may or may not be completely functioning at any point. **!! Work in Progress !!**

This version of the CChess Engine uses sockets for greater inter-platform compatibility. The engine hosts a socket server that can be connected to by any client. The server is cross-platform and uses a strategy pattern to select and load OS-specific socket code. There are separate client programs for both Windows and Linux.

## Running the binaries
Everything you need to run the chess engine is in the bin/ folder specific to your operating system.
Run the server first then run the client.

### Windows:
Use the C key on the windows client to connect to the server.
Use T key to send a test packet and check on the server. Pressing C again will disconnect you (and again will reconnect...).

### Linux:
(coming soon...)

## Compilation

# TIP
Rather then making your own binary, there are pre-included binaries for linux and windows inside of the /bin folder. If you are not interested in contributing code and only want to run the program, everything you need is in the bin/ folder.

### Windows:
- The executable requires the SDL2 graphics library, but all dlls/headers are included. No installation is required.
- Requires a windows configured Mingw64 toolchain.
- Run `make` in the root directory.

### Linux:
- Requires the Ncurses graphics library, is terminal only application.
- Install Ncurses using your package manager.
- Ensure that your terminal font is monospace.
- Run `make` in the root directory.

## Build and Execution

When you execute any of the Makefiles in the root of the project, depending on your operating system, either the `/linuxclient` or `/windowsclient` folders will be built. Your client & server executables will be found in the subfolder for your operating system.

To use the chess engine:

2. Run the chess engine server (`CChessServer`) generated at your bin/OS folder.
3. Connect to the chess engine with a client (in either `/linuxclient` or `/windowsclient`).
4. ... (nothing else works right now, send a test packet?) ...
5. profit?

By default, the port used for communication is 50123, as set in the `defs.h` file in the `/src` folder of any executable. The port is determined at compile time and can be set in the `defs.h` before building if you want a unique one for your server/clients (which can be a bit annoying **WIP**).
** the default IP is currently automatically set to local host, will soon be allowing you to change that... **

