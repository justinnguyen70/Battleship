//-----------------------------------------------------------------------------
//	File:           CPlayer.h
//
//	class:		CPlayer
//-----------------------------------------------------------------------------
#ifndef CPLAYER_H
#define CPLAYER_H

#include <fstream>
#include <climits>

#include "console.h"
#include "CShipInfo.h"
#include "CCell.h"
//-----------------------------------------------------------------------------
//	Title:          CPlayer  Class
//
//	Description:    This file contains the class definition for CPlayer 
//
//	Programmer:    TEAM ONE(KOUASSI KOUASSI,JUSTIN NGUYEN, JORDAN J PALADINO
//								David Schmeling)
//   
//	Date:           3/12/2017
// 
//	Version:	1.00
//   
//	Environment:	PC 
//				Software:   MS Windows 10
//				Compiles under Microsoft Visual C++.Net 2015
//
//	class CCell:
//
//	Properties:
//				CCell *m_gameGrid : pointer to CCell
//				playerSlot m_whichPlayer : current player
//				short m_piecesLeft : pieces left
//				CShipInfo m_ships[SHIP_SIZE]: CShipInfo array
//				gameSize m_size : size of the game
//				bool m_constructed : constructed or not
//
//	Methods:
//	
//		inline:
//	
//			CPlayer(playerSlot whichPlayer = FIRST, gameSize size = NORMAL) :
//					m_whichPlayer(whichPlayer), m_size(size), m_constructed(true)
//				initializes m_whichPlayer to FIRST, m_size to normal, and m_constructed 
//				to true, and also sets up the ships and allocate memory
//					parameter
//						playerSlot whichPlayer = FIRST-- the actual player 
//						gameSize size = NORMAL-- the game size
//
//			CPlayer(const CPlayer &player) : m_constructed(false) 
//				initialize m_constructed to false
//				set CPlayer to player
//					parameter
//						const CPlayer &player -- a const ref to CPlayer object
//
//				~CPlayer() --destructor
//					calls deleteMemory() to delete the allocated memory
//
//				short getPeicesLeft( ) 
//					returns m_piecesLeft
//				void setPeicesLeft(short count) 
//					set m_piecesLeft to count
//						parameter
//							short count-- the count on pieces left
//
//
//		non-inline:
//			bool loadShip(const CShipInfo &info);
//				load the ships
//			bool removeShip(unsigned short x, unsigned short y);
//				remove a ship
//			void randomizeGrid();
//				randomized the grid
//			void setupShips( );
//				setup ships
//			void clearGrid(gridSection section = YOURS);
//				clear grid
//			bool saveGrid(const std::string &location);
//				save the grid
//			bool loadGrid(const std::string &location);
//				load the grid
//			bool withinBounds(short x, short y, const Ship &ship = noShip, 
//								const CDirection &direction = vertical);
//				if ship is withing the bound of the grid size
//			bool validLocation(short x, short y, const Ship &ship, const CDirection &direction);
//				validates location
//			CShipInfo* getShip(Ship ship) { return &m_ships[ship]; }
//				get a ship
//			CCell& getCell(unsigned short x, unsigned short y, gridSection section = YOURS);
//				get a cell
//			void setCell(unsigned short x, unsigned short y, const Ship &ship, 
//							gridSection section = YOURS);
//				set a cell
//			void clearCell(unsigned short x, unsigned short y, gridSection section = YOURS);
//				clear a cell
//			hitState hitCell(unsigned short x, unsigned short y);
//				find the hit state
//			void printGrid(console::ConsoleBuffer &out, int x, int y, gridSection section);
//				print a grid to the consoleBuffer
//			static void printGrid(console::ConsoleBuffer &out, int x, int y, gameSize size);
//				print a grid to the consoleBuffer
//			void printGrid(gridSection section, std::ostream &out = std::cout);
//				print a grid to the std ostream
//			static void printGrid(gameSize size, std::ostream &out = std::cout);
//				print a grid to the std ostream
//			CPlayer& operator=(const CPlayer &player);
//				operator equal 
//    History Log:
//          	3/12/2017  PB  completed version 1.0
//-----------------------------------------------------------------------------
namespace sinkFleet {

    static const short SIZE_NAME_ARRAY_SIZE = 3;
    static const char* sizeNames[SIZE_NAME_ARRAY_SIZE] = {
        "Small",
        "Normal",
        "Large",
    };

    enum gameSize {
        SMALL = -1,
        NORMAL,
        LARGE,
    };

    static const short BASE_GRID_SIZE = 10;
    static const short BASE_SCALE_SIZE = 2;

    static const short PLAYER_SIZE = 2;
    static const char* playerName[PLAYER_SIZE] = {
        "First",
        "Second",
    };

    enum playerSlot {
        FIRST, SECOND
    };

    enum gridSection {
        YOURS, THEIRS
    };

    enum hitState {
        MISS, FOUND, SUNK
    };

    class CPlayer {
    public:
        CPlayer(playerSlot whichPlayer = FIRST, gameSize size = NORMAL) :
            m_whichPlayer(whichPlayer), m_size(size), m_constructed(true) {
			setupShips( );
			allocMemory( );
		}
        CPlayer(const CPlayer &player) :
            m_constructed(false) {
            *this = player;
        }
        ~CPlayer( ) {
            deleteMemory( );
        }

        bool loadShip(const CShipInfo &info);
        bool removeShip(unsigned short x, unsigned short y);
        void randomizeGrid( );

        gameSize getSize( ) { return m_size; }

        void setupShips( );
		void clearGrid(gridSection section = YOURS);

        short getPeicesLeft( ) { return m_piecesLeft; }
        void setPeicesLeft(short count) { m_piecesLeft = count; }

        bool saveGrid(const std::string &location);
        bool loadGrid(const std::string &location);

        bool withinBounds(short x, short y, const Ship &ship = noShip, const CDirection &direction = vertical);
        bool validLocation(short x, short y, const Ship &ship, const CDirection &direction);

        CShipInfo* getShip(Ship ship) { return &m_ships[ship]; }

        CCell& getCell(unsigned short x, unsigned short y, gridSection section = YOURS);
        void setCell(unsigned short x, unsigned short y, const Ship &ship, gridSection section = YOURS);
        void clearCell(unsigned short x, unsigned short y, gridSection section = YOURS);
        hitState hitCell(unsigned short x, unsigned short y);

        void printGrid(console::ConsoleBuffer &out, int x, int y, gridSection section);
        static void printGrid(console::ConsoleBuffer &out, int x, int y, gameSize size);

        void printGrid(gridSection section, std::ostream &out = std::cout);
        static void printGrid(gameSize size, std::ostream &out = std::cout);

        CPlayer& operator=(const CPlayer &player);

    private:
        void allocMemory( );
        void deleteMemory( );

        CCell *m_gameGrid;

        playerSlot m_whichPlayer;
        short m_piecesLeft;
        CShipInfo m_ships[SHIP_SIZE];
        gameSize m_size;

        bool m_constructed;
    };
}

#endif // !CPLAYER_H
