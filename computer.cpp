#include "computer.h"

namespace sinkFleet {
    hitState Computer::run(bool process) {
        hitState state = MISS;
        if(cycles < 0 && process) {
            short size = BASE_GRID_SIZE + BASE_SCALE_SIZE * currPlayer->getSize( );
            if(workX <= -1 || workY <= -1) {
                pickSpot( );
            } else {
                bool good = false;
                do {
                    lastX = currX = lastX + face * (1 - dir);
                    lastY = currY = lastY + face * dir;
                    if(currPlayer->withinBounds(currX, currY))
                        good = true;
                    else
                        if(found == 0) {
                            oppositeSide( );
                            found++;
                        } else if(found == 1) {
                            changeDirection( );
                            found = 0;
                        }
                } while(!good);
            }
            CCell &theirCell = oppPlayer->getCell(currX, currY);
            CCell &yourCell = currPlayer->getCell(currX, currY, THEIRS);

            if(yourCell.getShip( ) == nullptr) {
                state = oppPlayer->hitCell(currX, currY);

                switch(state) {
                default: case MISS:
                    currPlayer->setCell(currX, currY, MISSED, THEIRS);
                    break;
                case SUNK: case FOUND:
                    currPlayer->setCell(currX, currY, HIT, THEIRS);
                    break;
                }
                if(state == FOUND && workX <= -1 && workY <= -1) {
                    workX = lastX = currX;
                    workY = lastY = currY;
                    currShip = theirCell.getShip( );
                    pickDirection( );
                } else if(state == SUNK) {
                    setWorkPos(-1, -1);
                    currShip = nullptr;
                } else if(state == MISS) {
                    if(found == 0) {
                        oppositeSide( );
                        found++;
                    } else if(found == 1) {
                        changeDirection( );
                        found = 0;
                    }
                }
                totalCells++;
            } else if(theirCell.getShip( ) != currShip) {
                if(found == 0) {
                    oppositeSide( );
                    found++;
                } else if(found == 1) {
                    changeDirection( );
                    found = 0;
                }
                state = run(process);
            }

            cycles = cycleCount;
        } else if(cycles > cycleCount) {
            cycles = cycleCount;
        } else {
            cycles--;
        }
        return state;
    }

    void Computer::reset( ) {
        setCurrPos(-1, -1);
        setWorkPos(-1, -1);
        setLastPos(-1, -1);
        setScanPos(0, 0);
        found = 0;
        currShip = nullptr;
        face = 0;
        cycles = -1;
        dir = direction::HORIZONTAL;
        totalCells = 0;
    }

    void Computer::pickSpot( ) {
        short size = BASE_GRID_SIZE + BASE_SCALE_SIZE * currPlayer->getSize( );
        bool found = false;
        do {
            if(totalCells < 2 * (size * size) / 3) {
                setCurrPos(rand( ) % size, rand( ) % size);
            } else {
                CCell cell;
                do {
                    do {
                        cell = currPlayer->getCell(scanX, scanY, THEIRS);
                        scanX++;
                    } while((scanX %= size) && cell.getShip( ) != nullptr);
                    if(scanX == 0)
                        scanY++;
                } while(cell.getShip( ) != nullptr);
                setCurrPos(cell.getCol( ), cell.getRow( ));
            }
            CCell &cell = currPlayer->getCell(currX, currY, THEIRS);
            if(cell.getShip( ) != nullptr || cell.isTested( ))
                continue;

            short smallest = 10;
            for(short i = 1; i < SHIP_SIZE - 2; i++) {
                if(oppPlayer->getShip(static_cast<ship_type>(i))->getPiecesLeft( ) > 0) {
                    if(shipSize[i] < smallest) {
                        smallest = shipSize[i];
                    }
                }
            }

            bool good = false;
            for(short i = 0; i < 4 && !good; i++) {
                bool tiny = false;
                CCell curCell;
                short dirX = i == 1 ? 1 : i == 3 ? -1 : 0;
                short dirY = i == 0 ? -1 : i == 2 ? 1 : 0;

                for(short j = 0; j < smallest && !tiny; j++) {
                    if(currPlayer->withinBounds(currX + j * dirX, currY + j * dirY)) {
                        curCell = currPlayer->getCell(currX + j * dirX, currY + j * dirY, THEIRS);
                        if(curCell.getShip( ) != nullptr &&
                            curCell.getShip( )->getShipType( ) == MISSED) {
                            tiny = true;
                        }
                        if(curCell.getShip( ) != nullptr &&
                            curCell.getShip( )->getShipType( ) == HIT) {
                            if(curCell.getShip( )->isSunk( )) {
                                tiny = true;
                            }
                        }
                    } else {
                        tiny = true;
                    }
                }
                if(!tiny)
                    good = true;
            }
            if(good)
                found = true;
            else
                cell.setTested( );
        } while(!found);
    }

    void Computer::pickDirection( ) {
        dir = static_cast<direction>(rand( ) % 2);
        face = 1 - 2 * (rand( ) % 2);
        found = 0;
    }

    void Computer::oppositeSide( ) {
        lastX = workX;
        lastY = workY;
        face = -face;
    }

    void Computer::changeDirection( ) {
        lastX = workX;
        lastY = workY;
        dir = static_cast<direction>(1 - static_cast<direction>(dir));
        face = 1 - 2 * (rand( ) % 2);
    }
}