//-----------------------------------------------------------------------------
//	File:           CDirection .h
//
//	class:		CDirection 
//-----------------------------------------------------------------------------
#ifndef CDIRECTION_H
#define CDIRECTION_H
#include "console.h"
//-----------------------------------------------------------------------------
//	Title:          CDirection  Class
//
//	Description:    This file contains the class definition for CDirection 
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
//				ship_direction m_direction: direction of the ship
//				const CDirection horizontal(HORIZONTAL): HORIZONTAL direction 
//				const CDirection vertical(VERTICAL):VERTICAL direction
//
//	Methods:
//	
//		inline:
//			
//			CDirection(direction dir = HORIZONTAL)
//				initialize m_direction to HORIZONTAL
//				paramaters:
//					direction dir : the direction of the ship
//
//				unsigned short getRow( ) const -- accessor
//				unsigned short getCol() const -- accessor
//
//				void setTested() -- mutator
//					set m_tested to true
//				bool isTested() const { return m_tested; }
//					return m_tested
//
//				CShipInfo* getShip( )
//					return currentShip
//
//			void setShip(CShipInfo *ship)
//					set the current ship to ship
//						parameter
//							CShipInfo *ship --pointer to a CShipInfo object
//
//			void print(std::ostream &out = std::cout) const
//				prints out this object to the stream
//						parameter
//							ostream & sout  -- the stream used for output
//
//			void print(console::ConsoleBuffer &out) const
//				prints using a console buffer
//						parameter
//							console::ConsoleBuffer &out
//								a console buffer
//
//			void printName(std::ostream &out = std::cout) const
//				prints the name of the actual direction to std ouptut
//					parameter 
//						std::ostream &out = std::cout
//
//			operator direction() const 
//				return m_direction
//
//			 inline std::ostream& operator<<(std::ostream& os, const CDirection& in)
//				output stream to the std buffer
//					parameter
//						std::ostream& os -- the std buffer
//						const CDirection& in -- a reference to  CDirection
//
//			inline console::ConsoleBuffer& operator<<(console::ConsoleBuffer& os, const CDirection& in)
//					output stream to the console buffer
//					parameter
//						console::ConsoleBuffer& os -- the console Buffer buffer
//						const CDirection& in -- a reference to  CDirection
//
//
//    History Log:
//          	3/12/2017  PB  completed version 1.0
//-----------------------------------------------------------------------------
namespace sinkFleet {

	enum ship_direction {
		HORIZONTAL,
		VERTICAL,
	};
	typedef ship_direction direction;

    static const short DIR_SIZE = 2;

    static const char dirChar[DIR_SIZE] = {
		'H', 'V'
	};

    static const char* dirName[DIR_SIZE] = {
		"Horizontal",
		"Vertical",
	};

	class CDirection {
	public:
		CDirection(direction dir = HORIZONTAL)
			: m_direction(dir) { }

		void print(std::ostream &out = std::cout) const {
			out << dirChar[m_direction];
		}
		void print(console::ConsoleBuffer &out) const {
			out << dirChar[m_direction];
		}

		void printName(std::ostream &out = std::cout) const {
			out << dirName[m_direction];
		}
		void printName(console::ConsoleBuffer &out) const {
			out << dirName[m_direction];
		}

		operator direction() const {
			return m_direction;
		}

	private:
		ship_direction m_direction;
	};

    inline std::ostream& operator<<(std::ostream& os, const CDirection& in) {
        in.print(os);
        return os;
    }

    inline console::ConsoleBuffer& operator<<(console::ConsoleBuffer& os, const CDirection& in) {
        in.print(os);
        return os;
    }

	const CDirection horizontal(HORIZONTAL);
	const CDirection vertical(VERTICAL);
}

#endif // !CDIRECTION_H
