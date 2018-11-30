#ifndef GAME_H
#define GAME_H

#include "console.h"
#include "CPlayer.h"
#include "computer.h"

namespace sinkFleet {

    class Game {
    public:
        Game( ) :
            players { }, comp(&players[1], &players[0]), turn(FIRST),
            finishPlay(false), winner(false) {
        }

        void setupGame(gameSize size);

        gameSize getSize( ) const { return size; }
        playerSlot getTurn( ) const { return turn; }
        Computer& getComputer( ) { return comp; };
        CPlayer& getPlayer(playerSlot slot) { return players[slot]; }

        void setTurn(playerSlot player) {
            turn = player;
        }

        bool getFinished( ) { return finishPlay; }
        void setFinished(bool set) { finishPlay = set; }

        bool isWinner( ) { return winner; }
        void setWinner(bool set) { winner = set; }

        void setCreatePlay( ) {
            create = true; computer = false;
        }
        void setComputerPlay( ) {
            create = false; computer = true;
        }
        void setPlayerPlay( ) {
            create = false; computer = false;
        }

        bool isCreatePlay( ) {
            return create && !computer;
        }
        bool isComputerPlay( ) {
            return !create && computer;
        }
        bool isPlayerPlay( ) {
            return !create && !computer;
        }

    private:
        gameSize size;
        CPlayer players[PLAYER_SIZE];
        Computer comp;
        playerSlot turn;

        bool create;
        bool computer;
        bool finishPlay;
        bool winner;
    };
}

#endif // GAME_H