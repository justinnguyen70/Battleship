#include "GameSystem.h"

namespace sinkFleet {

    void GameSystem::resetControls(short select, short x, short y) {
        selector = select;
        cursorX = x;
        cursorY = y;
    }

    void GameSystem::update(console::Keyboard &keyboard) {
        if(keyboard.keyPressed(VK_ESCAPE, console::SINGLE)) {
            running(false);
        }

        if(messageTimer == 0) {
            message = "";
        } else if(messageTimer != -1) {
            messageTimer--;
        }

        if(state == MAIN_MENU) {
            if(game.getPlayer(FIRST).getPeicesLeft( ) <= 0
                && game.getComputer( ).getCycles( ) <= 0) {
                game.getPlayer(FIRST).clearGrid( );
                game.getPlayer(SECOND).clearGrid(THEIRS);
                game.getPlayer(FIRST).randomizeGrid( );
                game.getComputer( ).reset( );
                game.getComputer( ).setCycles(80);
            } else {
                game.getComputer( ).run(true);
            }

            if((keyboard.keyPressed(VK_UP, console::REPEAT) ||
                keyboard.keyPressed('W', console::REPEAT))
                && selector > 0) selector--;
            if((keyboard.keyPressed(VK_DOWN, console::REPEAT) ||
                keyboard.keyPressed('S', console::REPEAT))
                && selector < 3) selector++;
            if(keyboard.keyPressed(VK_RETURN, console::SINGLE)) {
                switch(selector) {
                case 0: game.setPlayerPlay( ); state = SELECT_SIZE; break;
                case 1: game.setComputerPlay( ); state = SELECT_SIZE; break;
                case 2: game.setCreatePlay( ); state = SELECT_SIZE; break;
                default: running(false); break;
                }
                resetControls(1, 0, 0);
            }
        } else if(state == SELECT_SIZE) {
            if((keyboard.keyPressed(VK_UP, console::REPEAT) ||
                keyboard.keyPressed('W', console::REPEAT))
                && selector > 0) selector--;
            if((keyboard.keyPressed(VK_DOWN, console::REPEAT) ||
                keyboard.keyPressed('S', console::REPEAT))
                && selector < 3) selector++;
            if(keyboard.keyPressed(VK_BACK, console::SINGLE)) {
                resetControls( );
                game.setupGame(NORMAL);
                game.getPlayer(FIRST).randomizeGrid( );
                game.getComputer( ).reset( );
                game.getComputer( ).setCycles(80);
                state = MAIN_MENU;
            }
            if(keyboard.keyPressed(VK_RETURN, console::SINGLE)) {
                switch(selector) {
                case 0: game.setupGame(SMALL); state = CREATE_GRID; resetControls(1); break;
                case 1: game.setupGame(NORMAL); state = CREATE_GRID; resetControls(1); break;
                case 2: game.setupGame(LARGE); state = CREATE_GRID; resetControls(1); break;
                default:  resetControls( ); state = MAIN_MENU; resetControls( ); break;
                }
                turn = FIRST;
                message = "";
                firstDone = false;
                secondDone = false;
            }
        } else if(state == CREATE_GRID) {
            int size = BASE_GRID_SIZE + BASE_SCALE_SIZE * game.getSize( );
            if(keyboard.keyPressed('R', console::SINGLE)) {
                dir = static_cast<direction>(1 - dir);
                if(!game.getPlayer(turn).withinBounds(cursorX, cursorY,
                    static_cast<ship_type>(selector), dir)) {
                    cursorX = dir ? cursorX : size - shipSize[selector];
                    cursorY = dir ? size - shipSize[selector] : cursorY;
                }
            }
            if((keyboard.keyPressed(VK_UP, console::REPEAT) ||
                keyboard.keyPressed('W', console::REPEAT))
                && cursorY > 0) cursorY--;
            if((keyboard.keyPressed(VK_DOWN, console::REPEAT) ||
                keyboard.keyPressed('S', console::REPEAT)
                && !keyboard.keyPressed(VK_CONTROL, console::HOLD))
                && cursorY < size - (dir && selector != 0 ?
                    shipSize[selector] : 1)) cursorY++;
            if((keyboard.keyPressed(VK_LEFT, console::REPEAT) ||
                keyboard.keyPressed('A', console::REPEAT))
                && cursorX > 0) cursorX--;
            if((keyboard.keyPressed(VK_RIGHT, console::REPEAT) ||
                keyboard.keyPressed('D', console::REPEAT))
                && cursorX < size - (1 - dir && selector != 0 ?
                    shipSize[selector] : 1)) cursorX++;
            if(keyboard.keyPressed(VK_CONTROL, console::REPEAT) &&
                keyboard.keyPressed('O', console::REPEAT)) {
                state = LOAD_GRID;
                message = "";
                messageTimer = -1;
            }
            if(!game.isCreatePlay( )) {
                if(keyboard.keyPressed(VK_CONTROL, console::REPEAT) &&
                    keyboard.keyPressed('S', console::REPEAT)) {
                    if((1 - turn) && firstDone || turn && secondDone) {
                        state = SAVE_GRID;
                        message = "";
                        messageTimer = -1;
                    } else {
                        message = "You can not save yet";
                        messageTimer = 80;
                    }
                }
            }

            if(keyboard.keyPressed(VK_SPACE, console::SINGLE)) {
                if(selector == 0) {
                    CShipInfo *info = game.getPlayer(turn).getCell(cursorX, cursorY).getShip( );
                    if(info != nullptr) {
                        selector = info->getShipType( );
                        cursorX = info->getCol( );
                        cursorY = info->getRow( );
                        dir = info->getDirection( );
                        game.getPlayer(turn).removeShip(info->getCol( ), info->getRow( ));
                    }
                } else {
                    if(game.getPlayer(turn).validLocation(cursorX, cursorY,
                        static_cast<ship_type>(selector), dir)) {
                        if(game.getPlayer(turn).loadShip(CShipInfo(dir, cursorX,
                            cursorY, static_cast<ship_type>(selector)))) {
                            if((1 - turn) && firstDone || turn && secondDone)
                                selector = 0;
                            else {
                                ++selector %= 6;
                                if(shipSize[selector] + cursorX * (1 - dir) + cursorY * dir - size > 0) {
                                    cursorX = dir ? cursorX : size - shipSize[selector];
                                    cursorY = dir ? size - shipSize[selector] : cursorY;
                                }
                            }
                        }
                        if(selector == 0) {
                            if(1 - turn) firstDone = true;
                            if(turn) secondDone = true;
                        }
                    } else {
                        message = "You can not place that there";
                        messageTimer = 80;
                    }
                }
            }

            if(keyboard.keyPressed(VK_TAB, console::SINGLE)) {
                game.getPlayer(turn).clearGrid( );
                game.getPlayer(turn).randomizeGrid( );
                selector = 0;
                if(1 - turn) firstDone = true;
                if(turn) secondDone = true;
            }
            if(keyboard.keyPressed(VK_BACK, console::SINGLE)) {
                if(turn == FIRST) {
                    resetControls(1);
                    state = SELECT_SIZE;
                } else {
                    turn = FIRST;
                    resetControls( );
                }
            }
            if(keyboard.keyPressed(VK_RETURN, console::SINGLE)) {
                if(selector == 0 && ((1 - turn) && firstDone || turn && secondDone)) {
                    if(turn == FIRST && game.isPlayerPlay( )) {
                        turn = static_cast<playerSlot>(turn + 1);
                        if(!secondDone) {
                            game.getPlayer(SECOND).clearGrid( );
                            resetControls(1);
                        } else
                            resetControls( );
                    } else if(game.isCreatePlay( )) {
                        state = SAVE_GRID;
                        message = "";
                        messageTimer = -1;
                    } else {
                        if(game.isComputerPlay( )) {
                            game.getPlayer(SECOND).clearGrid( );
                            game.getPlayer(SECOND).randomizeGrid( );
                        }
                        resetControls( );
                        state = CREATE_CONFIRM;
                    }
                } else {
                    message = "You have not placed all of your ships";
                    messageTimer = 80;
                }
            }
        } else if(state == CREATE_CONFIRM) {
            if((keyboard.keyPressed(VK_LEFT, console::REPEAT) ||
                keyboard.keyPressed('A', console::REPEAT))
                && selector > 0) selector--;
            if((keyboard.keyPressed(VK_RIGHT, console::REPEAT) ||
                keyboard.keyPressed('D', console::REPEAT))
                && selector < 1) selector++;
            if(keyboard.keyPressed(VK_RETURN, console::SINGLE)) {
                if(selector == 1) {
                    state = BETWEEN_PLAY;
                    game.setTurn(SECOND);
                    resetControls( );
                } else {
                    state = CREATE_GRID;
                    resetControls( );
                }
            }
        } else if(state == BETWEEN_PLAY) {
            if(game.isPlayerPlay( ) || game.isComputerPlay( ) && game.getTurn( ) == SECOND) {
                if(keyboard.keyPressed(VK_RETURN, console::SINGLE)) {
                    game.setTurn(static_cast<playerSlot>(1 - game.getTurn( )));
                    resetControls( );
                    state = PLAY_GAME;
                }
                message = "";
            } else if(game.isComputerPlay( )) {
                if(game.getComputer( ).getCycles( ) <= 0) {
                    game.setTurn(static_cast<playerSlot>(1 - game.getTurn( )));
                    state = PLAY_GAME;
                    game.getComputer( ).setCycles(game.getComputer( ).getCycleCount( ));
                } else
                    game.getComputer( ).setCycles(game
                        .getComputer( ).getCycles( ) - 1);
                message = "";
            }
        } else if(state == LOAD_GRID) {
            unsigned char c = keyboard.getNextReadKey( );
            if(c != 0)
                message += c;
            if(keyboard.keyPressed(VK_BACK, console::REPEAT) && message.size( ) > 0)
                message.erase(message.size( ) - 1, 1);
            else if(keyboard.keyPressed(VK_BACK, console::SINGLE))
                state = CREATE_GRID;
            if(keyboard.keyPressed(VK_RETURN, console::SINGLE)) {
                if(message != "") {
                    bool loaded = game.getPlayer(turn).loadGrid(message);
                    if(loaded) {
                        message = "Successfully loaded " + message;
                        messageTimer = 80;
                    } else {
                        message = "Could not load " + message;
                        messageTimer = 80;
                    }
                    state = CREATE_GRID;
                    if(1 - turn && loaded) {
                        firstDone = true;
                        selector = 0;
                    }
                    if(turn && loaded) {
                        secondDone = true;
                        selector = 0;
                    }
                }
            }
        } else if(state == SAVE_GRID) {
            unsigned char c = keyboard.getNextReadKey( );
            if(c != 0)
                message += c;
            if(keyboard.keyPressed(VK_BACK, console::REPEAT) && message.size( ) > 0)
                message.erase(message.size( ) - 1, 1);
            else if(keyboard.keyPressed(VK_BACK, console::SINGLE))
                state = CREATE_GRID;
            if(keyboard.keyPressed(VK_RETURN, console::SINGLE)) {
                if(message != "") {
                    bool saved = game.getPlayer(turn).saveGrid(message);
                    if(saved) {
                        message = "Successfully saved " + message;
                        messageTimer = 80;
                    } else {
                        message = "Could not save " + message;
                        messageTimer = 80;
                    }
                    if(game.isCreatePlay( ) && saved)
                        state = MAIN_MENU;
                    else {
                        state = CREATE_GRID;
                    }
                }
            }
        } else if(state == PLAY_GAME) {
            if(game.isComputerPlay( ) && game.getTurn( ) == FIRST || game.isPlayerPlay( )) {
                int size = BASE_GRID_SIZE + BASE_SCALE_SIZE * game.getSize( );
                if(!game.getFinished( )) {
                    if((keyboard.keyPressed(VK_UP, console::REPEAT) ||
                        keyboard.keyPressed('W', console::REPEAT))
                        && cursorY > 0) cursorY--;
                    if((keyboard.keyPressed(VK_DOWN, console::REPEAT) ||
                        keyboard.keyPressed('S', console::REPEAT))
                        && cursorY < size - 1) cursorY++;
                    if((keyboard.keyPressed(VK_LEFT, console::REPEAT) ||
                        keyboard.keyPressed('A', console::REPEAT))
                        && cursorX > 0) cursorX--;
                    if((keyboard.keyPressed(VK_RIGHT, console::REPEAT) ||
                        keyboard.keyPressed('D', console::REPEAT))
                        && cursorX < size - 1) cursorX++;
                }
                if(keyboard.keyPressed(VK_RETURN, console::SINGLE) ||
                    keyboard.keyPressed(VK_SPACE, console::SINGLE)) {
                    if(game.isWinner( )) {
                        state = PLAY_AGAIN;
                        resetControls( );
                    } else if(game.isComputerPlay( ) && game.getTurn( ) == FIRST || game.isPlayerPlay( )) {
                        CShipInfo *info = game.getPlayer(game.getTurn( )).getCell(cursorX, cursorY, THEIRS).getShip( );
                        if(info == nullptr && !game.getFinished( )) {
                            playerSlot opp = static_cast<playerSlot>(1 - game.getTurn( ));
                            hitState state = game.getPlayer(opp).hitCell(cursorX, cursorY);
                            CShipInfo *oppInfo = game.getPlayer(opp).getCell(cursorX, cursorY, YOURS).getShip( );
                            switch(state) {
                            default:
                            case MISS:
                                game.getPlayer(game.getTurn( )).setCell(cursorX, cursorY, MISSED, THEIRS);
                                message = "You have missed, Press enter to continue";
                                messageTimer = -1;
                                game.setFinished(true);
                                break;
                            case SUNK:
                                message = "Their " + static_cast<std::string>(shipName[oppInfo->getShipType( )])
                                    + " has been sunk";
                                if(game.getPlayer(opp).getPeicesLeft( ) <= 0) {
                                    message = " Player " + std::to_string(1 + game.getTurn( ))
                                        + " has won, Press enter to continue.";
                                    game.setWinner(true);
                                    game.setFinished(true);
                                } else
                                    messageTimer = 80;
                                game.getPlayer(game.getTurn( )).setCell(cursorX, cursorY, HIT, THEIRS);
                                break;
                            case FOUND:
                                message = "You have hit a ship";
                                messageTimer = 80;
                                game.getPlayer(game.getTurn( )).setCell(cursorX, cursorY, HIT, THEIRS);
                                break;
                            }
                        } else if(game.getFinished( )) {
                            state = BETWEEN_PLAY;
                            resetControls( );
                            game.setFinished(false);
                            game.getComputer( ).setCycles(game.getComputer( ).getCycleCount( ));
                        } else {
                            message = "You already have hit there";
                            messageTimer = 80;
                        }
                    }
                }
            } else if(game.isComputerPlay( ) && !game.isWinner( )) {
                bool turnTaken = game.getComputer( ).getCycles( ) == -1;
                hitState state = game.getComputer( ).run(!game.getFinished( ));
                if(turnTaken && !game.getFinished( )) {
                    playerSlot opp = static_cast<playerSlot>(1 - game.getTurn( ));
                    CShipInfo *oppInfo = game.getPlayer(opp).getCell(game.getComputer( )
                        .getCurrX( ), game.getComputer( ).getCurrY( ), YOURS).getShip( );
                    switch(state) {
                    default:
                    case MISS:
                        message = "The computer has missed";
                        messageTimer = -1;
                        game.setFinished(true);
                        game.getComputer( ).setCycles(game.getComputer( ).getCycleCount( ));
                        break;
                    case SUNK:
                        message = "The computer has sunk your " +
                            std::string(shipName[oppInfo->getShipType( )]);
                        if(game.getPlayer(opp).getPeicesLeft( ) <= 0) {
                            message = " The computer has won, Press enter to continue.";
                            game.setWinner(true);
                            game.setFinished(true);
                        } else
                            messageTimer = 80;
                        break;
                    case FOUND:
                        message = "The computer has hit one of your ships";
                        messageTimer = 80;
                        break;
                    }
                } else if(game.getFinished( ) && turnTaken) {
                    this->state = BETWEEN_PLAY;
                    resetControls( );
                    game.setFinished(false);
                }
            }

        } else if(state == PLAY_AGAIN) {
            if((keyboard.keyPressed(VK_LEFT, console::REPEAT) ||
                keyboard.keyPressed('A', console::REPEAT))
                && selector > 0) selector--;
            if((keyboard.keyPressed(VK_RIGHT, console::REPEAT) ||
                keyboard.keyPressed('D', console::REPEAT))
                && selector < 1) selector++;
            if(keyboard.keyPressed(VK_RETURN, console::SINGLE)) {
                if(selector == 1) {
                    state = SELECT_SIZE;
                    resetControls( );
                } else {
                    state = MAIN_MENU;
                    game.setupGame(NORMAL);
                    game.getPlayer(FIRST).randomizeGrid( );
                    game.getComputer( ).reset( );
                    game.getComputer( ).setCycles(80);
                    resetControls( );
                }
            }
        } else {
            state = MAIN_MENU;
            game.setupGame(NORMAL);
            game.getPlayer(FIRST).randomizeGrid( );
            game.getComputer( ).reset( );
            game.getComputer( ).setCycles(80);
            resetControls( );
        }
    }

    void GameSystem::draw(console::ConsoleBuffer &buffer) {
        if(state == MAIN_MENU) {
            int size = BASE_GRID_SIZE + BASE_SCALE_SIZE * gameSize::NORMAL;
            buffer(2, 1);
            buffer << "Welcome to Sink The Fleet 2" << console::endl << console::endl
                << console::setX(5) << "Play vs Player" << console::endl
                << console::setX(5) << "Play vs Computer" << console::endl
                << console::setX(5) << "Create Grid" << console::endl << console::endl
                << console::setX(5) << "Quit" << console::endl << console::endl
                << console::setX(2) << "Controls:" << console::endl
                << console::setX(5) << "Accept: Enter" << console::endl
                << console::setX(5) << "Up:     Up" << console::endl
                << console::setX(5) << "Down:   Down" << console::endl << console::endl
                << console::setX(5) << "Exit:   Esc" << console::endl;
            buffer(2, 3 + selector + (selector > 2 ? 1 : 0));
            buffer << ">";
            buffer(2, buffer.getHeight( ) - 3);
            buffer << "Created by: #include <group_name.h>";

            short gridShift = buffer.getWidth( ) - (size * 4 + 9);
            game.getPlayer(SECOND).printGrid(buffer, gridShift, 3, THEIRS);
            short cx = game.getComputer( ).getCurrX( ) > -1 ? game.getComputer( ).getCurrX( ) : 0;
            short cy = game.getComputer( ).getCurrY( ) > -1 ? game.getComputer( ).getCurrY( ) : 0;
            buffer(5 + gridShift + cx * 4, 5 + cy * 2);
            buffer << "X";
        } else if(state == SELECT_SIZE) {
            buffer(2, 1);
            buffer << "Please select a grid size:" << console::endl << console::endl;
            for(int i = 0; i < SIZE_NAME_ARRAY_SIZE; i++) {
                buffer << console::setX(5) << sizeNames[i] << console::endl;
            }
            buffer(4, 4 + SIZE_NAME_ARRAY_SIZE);
            buffer << console::setX(5) << "Return" << console::endl << console::endl
                << console::setX(2) << "Controls:" << console::endl
                << console::setX(5) << "Accept: Enter" << console::endl
                << console::setX(5) << "Up:     Up" << console::endl
                << console::setX(5) << "Down:   Down" << console::endl
                << console::setX(5) << "Return: Backspace" << console::endl << console::endl
                << console::setX(5) << "Exit:   Esc" << console::endl;
            buffer(2, 3 + selector + (selector > 2 ? 1 : 0));
            buffer << ">";
            gameSize grid = gameSize(selector > 2 ? 1 : selector - 1);
            int size = BASE_GRID_SIZE + BASE_SCALE_SIZE * grid;
            CPlayer::printGrid(buffer, buffer.getWidth( ) - (size * 4 + 5), 1, grid);
        } else if(state == CREATE_GRID) {
            int size = BASE_GRID_SIZE + BASE_SCALE_SIZE * game.getSize( );
            int sideInfoShift = size * 4 + 8;
            game.getPlayer(turn).printGrid(buffer, 2, 1, sinkFleet::YOURS);
            buffer(sideInfoShift, 1);
            if(game.isCreatePlay( )) {
                buffer << "Create grid layout";
            } else {
                buffer << "Player " << 1 + turn << "'s Ships";
            }
            buffer(sideInfoShift, 3);
            for(short i = 1; i < SHIP_SIZE - 2; i++) {
                buffer << console::setX(sideInfoShift) << (selector == i ? ">" : " ")
                    << " " << shipName[i] << console::endl;
            }
            buffer << console::endl << console::setX(sideInfoShift) << (selector == 0 ? ">" : " ")
                << " " << "Finished" << console::endl << console::endl;
            buffer.setFlags(console::flag::right);
            buffer << console::setX(sideInfoShift) << "Controls:" << console::endl
                << console::setX(sideInfoShift + 2) << "Accept: Enter" << console::endl
                << console::setX(sideInfoShift + 2) << "Move:" << console::endl
                << console::setX(sideInfoShift + 4) << "Arrow Keys" << console::endl
                << console::setX(sideInfoShift + 4) << "W A S D" << console::endl
                << console::setX(sideInfoShift + 2) << "Rotate: R" << console::endl
                << console::setX(sideInfoShift + 2) << "Place:  Space" << console::endl
                << console::setX(sideInfoShift + 2) << "Random: Tab" << console::endl
                << console::setX(sideInfoShift + 2) << "Return: Backspace" << console::endl
                << console::setX(sideInfoShift + 2) << "Load:   Ctrl O" << console::endl;
            if(!game.isCreatePlay( ))
                buffer << console::setX(sideInfoShift + 2) << "Save:   Ctrl S" << console::endl;
            buffer << console::endl << console::setX(sideInfoShift + 2) << "Exit:   Esc" << console::endl;

            if(selector == 0) {
                buffer(cursorX * 4 + 7, cursorY * 2 + 3);
                buffer << "X";
            } else {
                for(int i = 0; i < shipSize[selector]; i++) {
                    short dx = (cursorX + i * (1 - dir)) * 4 + 7;
                    short dy = (cursorY + i * dir) * 2 + 3;
                    buffer(dx, dy);
                    buffer << shipChar[selector];
                }
            }
            buffer(2, size * 2 + 4);
            buffer << message;
        } else if(state == CREATE_CONFIRM) {
            std::string sel = "";
            sel = sel + (selector == 0 ? "<" : " ") + "No" + (selector == 0 ? ">" : " ");
            sel = sel + " " + (selector == 1 ? "<" : " ") + "Yes" + (selector == 1 ? ">" : " ");
            buffer(buffer.getWidth( ) / 2 - 16, buffer.getHeight( ) / 2 - 1);
            buffer.setFlags(console::flag::center);
            buffer << console::setS(31) << "Are you sure you are finished?" << console::endl
                << console::endl << console::setX(buffer.getWidth( ) / 2 - 16) << console::setS(31) << sel;
            buffer.setFlags(console::flag::right);
        } else if(state == BETWEEN_PLAY) {
            buffer(buffer.getWidth( ) / 2 - 8, buffer.getHeight( ) / 2 - 1);
            buffer.setFlags(console::flag::center);
            if(game.isComputerPlay( ) && game.getTurn( ) == FIRST) {
                buffer << "Computer's Turn";
            } else {
                buffer << "Player " << 1 + (1 - game.getTurn( )) << "'s Turn" << console::endl
                    << console::endl << console::setX(buffer.getWidth( ) / 2 - 12) <<
                    "Press enter to continue";
            }
            buffer.setFlags(console::flag::right);
        } else if(state == LOAD_GRID) {
            short midX = buffer.getWidth( ) / 2;
            short midY = buffer.getHeight( ) / 2;
            buffer(midX - 18, midY - 1);
            buffer << "Please enter a location to load from"
                << console::endl << console::endl;
            buffer(midX - message.size( ) / 2, midY + 1);
            buffer << message + "_";
        } else if(state == SAVE_GRID) {
            short midX = buffer.getWidth( ) / 2;
            short midY = buffer.getHeight( ) / 2;
            buffer(midX - 18, midY - 1);
            buffer << "Please enter a location to save to"
                << console::endl << console::endl;
            buffer(midX - message.size( ) / 2, midY + 1);
            buffer << message + "_";
        } else if(state == PLAY_GAME) {
            int size = BASE_GRID_SIZE + BASE_SCALE_SIZE * game.getSize( );
            int sideInfoShift = size * 4 + 8;
            game.getPlayer(game.getTurn( )).printGrid(buffer, 2, 1, sinkFleet::THEIRS);
            buffer(sideInfoShift, 1);
            if(game.isComputerPlay( ) && 1 - game.getTurn( )) {
                buffer << "Computer's Ships";
            } else {
                buffer << "Player " << 1 + (1 - game.getTurn( )) << "'s Ships";
            }
            buffer(sideInfoShift, 3);
            playerSlot opp = static_cast<playerSlot>(1 - game.getTurn( ));
            for(short i = 1; i < SHIP_SIZE - 2; i++) {
                short count = game.getPlayer(opp).getShip(static_cast<ship_type>(i))->getPiecesLeft( );
                buffer << console::setX(sideInfoShift) << console::left << console::setS(17) << shipName[i] << " "
                    << (count > 0 ? "Floating" : "Sunk") << console::right << console::endl;
            }
            buffer << console::endl;
            buffer.setFlags(console::flag::right);
            if(game.isComputerPlay( ) && game.getTurn( ) == FIRST || game.isPlayerPlay( )) {
                buffer << console::setX(sideInfoShift) << "Controls:" << console::endl
                    << console::setX(sideInfoShift + 2) << "Accept:" << console::endl
                    << console::setX(sideInfoShift + 4) << "Enter" << console::endl
                    << console::setX(sideInfoShift + 4) << "Space" << console::endl
                    << console::setX(sideInfoShift + 2) << "Move:" << console::endl
                    << console::setX(sideInfoShift + 4) << "Arrow Keys" << console::endl
                    << console::setX(sideInfoShift + 4) << "W A S D" << console::endl
                    << console::endl << console::setX(sideInfoShift + 2) << "Exit:   Esc" << console::endl;
            }
            if(!game.getFinished( )) {
                short cx = game.getComputer( ).getCurrX( ) > -1 ? game.getComputer( ).getCurrX( ) : 0;
                short cy = game.getComputer( ).getCurrY( ) > -1 ? game.getComputer( ).getCurrY( ) : 0;
                if(game.isComputerPlay( ) && game.getTurn( ) == SECOND)
                    buffer(cx * 4 + 7, cy * 2 + 3);
                else
                    buffer(cursorX * 4 + 7, cursorY * 2 + 3);
                buffer << "X";
            }
            buffer(2, size * 2 + 4);
            buffer << message;
        } else if(state == PLAY_AGAIN) {
            std::string sel = "";
            sel = sel + (selector == 0 ? "<" : " ") + "No" + (selector == 0 ? ">" : " ");
            sel = sel + " " + (selector == 1 ? "<" : " ") + "Yes" + (selector == 1 ? ">" : " ");
            buffer(buffer.getWidth( ) / 2 - 13, buffer.getHeight( ) / 2 - 1);
            buffer.setFlags(console::flag::center);
            buffer << console::setS(27) << "Do you want to Play again?" << console::endl
                << console::endl << console::setX(buffer.getWidth( ) / 2 - 13) << console::setS(27) << sel;
            buffer.setFlags(console::flag::right);
        } else {

        }
    }

    bool GameSystem::running(bool set) {
        bool oldRun = gameRunning;
        gameRunning = set;
        return oldRun;
    }
}