#ifndef GAMESYSTEM_H
#define GAMESYSTEM_H

#include "console.h"
#include "game.h"

namespace sinkFleet {

    enum gameState {
        MAIN_MENU,
        SELECT_SIZE,
        CREATE_GRID,
        CREATE_CONFIRM,
        BETWEEN_PLAY,
        LOAD_GRID,
        SAVE_GRID,

        PLAY_GAME = 10,
        PLAY_AGAIN,

        RETURN_CREATE_GRID = -10,
    };


    class GameSystem {
    public:
        GameSystem( ) :
            gameRunning(true), selector(0), cursorX(0), cursorY(0),
            state(0), turn(FIRST), dir(HORIZONTAL), message( ),
            messageTimer(-1), firstDone(false), secondDone(false),
            game( ) {
            game.getPlayer(FIRST).randomizeGrid( );
        }

        void update(console::Keyboard &keyboard);
        void draw(console::ConsoleBuffer &buffer);

        bool running( ) { return gameRunning; }
        bool running(bool set);

    private:
        void resetControls(short select = 0, short x = 0, short y = 0);

        bool gameRunning;
        short selector;
        short cursorX;
        short cursorY;

        short state;
        playerSlot turn;
        CDirection dir;

        std::string message;
        short messageTimer;
        bool firstDone;
        bool secondDone;

        Game game;
    };
}
#endif // !GAMESYSTEM_H
