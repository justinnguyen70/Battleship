//-----------------------------------------------------------------------------
//	File:           Computer .h
//
//	class:		Computer 
//-----------------------------------------------------------------------------
#ifndef COMPUTER_H
#define COMPUTER_H

#include "CPlayer.h"
//-----------------------------------------------------------------------------
//	Title:          Computer  Class
//
//	Description:    This file contains the class definition for Computer 
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
//				ship_type m_type: a ship type
//
//	Methods:
//	
//		inline:
//			
//			 Computer::Computer(CPlayer *self, CPlayer *opponent) :
//				initializes currX(-1), currY(-1), workX(-1), workY(-1),
//				lastX(-1), lastY(-1), scanX(0), scanY(0),
//				found(0), currShip(nullptr), face(0),
//				cycles(-1), cycleCount(20), dir(direction::HORIZONTAL),
//				totalCells(0), currPlayer(self), and oppPlayer(opponent)
//					parameters
//						CPlayer *self-- player one
//						CPlayer *opponent-- player two
//
//			void setLastPos(int x, int y) 
//				set	lastX to x and lastY to y
//					parameter
//						int x-- x-axis
//						int y-- y-axis
//
//			void setCurrPos(int x, int y) 
//				set	currX to x and currY to y
//					parameter
//						int x-- x-axis
//						int y-- y-axis
//
//			short getCurrX() 
//				returns currX
//
//			short getCurrY() 
//				returns currY
//	
//			void setWorkPos(int x, int y) 
//				sets workX to x and workY to y;
//					parameter
//						int x-- x-axis
//						int y-- y-axis
//
//			void setScanPos(int x, int y) 
//				sets scanX to x; scanY to y
//					parameter
//						int x-- x-axis
//						int y-- y-axis
//
//			short getCycles( ) 
//				returns cycles
//
//			void setCycles(short count) 
//				sets cycles to count
//					parameter
//						count -- count cycles
//	
//			short getCycleCount() 
//				returns cycleCount
//
//			void setCycleCount(short count) 
//				sets cycleCount to count
//
//		non-inline:
//			hitState run(bool process);
//				the hit state 
//			void reset();
//				reset
//			 void pickSpot( );
//				pick a spot
//			 void pickDirection();
//				pick a direction
//			void oppositeSide();
//				opposite side
//			void changeDirection();
//				change direction
//			
//    History Log:
//          	3/12/2017  PB  completed version 1.0
//-----------------------------------------------------------------------------
namespace sinkFleet {

    class Computer {
    public:
        Computer::Computer(CPlayer *self, CPlayer *opponent) :
            currX(-1), currY(-1), workX(-1), workY(-1),
            lastX(-1), lastY(-1), scanX(0), scanY(0),
            found(0), currShip(nullptr), face(0),
            cycles(-1), cycleCount(20), dir(direction::HORIZONTAL),
            totalCells(0), currPlayer(self), oppPlayer(opponent) {
            if(self->getSize( ) != opponent->getSize( ))
                throw std::invalid_argument("The two players give were not "
                    "setup with the same size");
        }

        void setLastPos(int x, int y) {
            lastX = x; lastY = y;
        }
        void setCurrPos(int x, int y) {
            currX = x; currY = y;
        }
        short getCurrX( ) { return currX; }
        short getCurrY( ) { return currY; }
        void setWorkPos(int x, int y) {
            workX = x; workY = y;
        }
        void setScanPos(int x, int y) {
            scanX = x; scanY = y;
        }

        hitState run(bool process);
        void reset( );

        short getCycles( ) { return cycles; }
        void setCycles(short count) { cycles = count; }
        short getCycleCount( ) { return cycleCount; }
        void setCycleCount(short count) { cycleCount = count; }

    private:
        void pickSpot( );
        void pickDirection( );
        void oppositeSide( );
        void changeDirection( );

        short cycles;
        short cycleCount;
        short lastX;
        short lastY;
        short currX;
        short currY;
        short workX;
        short workY;
        short scanX;
        short scanY;
        short face;
        short found;
        CDirection dir;
        CShipInfo *currShip;
        CPlayer *currPlayer;
        CPlayer *oppPlayer;
        short totalCells;
    };
}
#endif // !COMPUTER_H