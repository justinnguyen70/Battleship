
#include "game.h"
namespace sinkFleet {

    void Game::setupGame(gameSize size) {
        players[FIRST] = CPlayer(FIRST, size);
        players[SECOND] = CPlayer(SECOND, size);
        comp = Computer(&players[SECOND], &players[FIRST]);
        this->size = size;
        turn = FIRST;
        winner = false;
        finishPlay = false;
    }
}
