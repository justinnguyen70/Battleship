//-----------------------------------------------------------------------------
//	File:           CCell.h
//
//	class:		CCell
//-----------------------------------------------------------------------------
#ifndef CCELL_H
#define CCELL_H
#include "console.h"
#include "CShipInfo.h"
//-----------------------------------------------------------------------------
//	Title:          CCell Class
//
//	Description:    This file contains the class definition for CCell
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
//				unsigned short m_row;:the row
//				unsigned short m_col: the row
//				bool m_tested
//				CShipInfo *currentShip: the current ship
//
//	Methods:
//	
//		inline:
//			
//			CCell(unsigned short col = 0, unsigned short row = 0) 
//				constructor for integer
//				intialize m_row, m_col, m_tested, and currentShip 
//				paramaters:
//					unsigned short col: the column
//					unsigned short row: the row
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
//				void setShip(CShipInfo *ship)
//					set the current ship to ship
//						parameter
//							CShipInfo *ship --pointer to a CShipInfo object
//
//				void print(std::ostream &out = std::cout) const
//					prints out this object to the stream
//						parameter
//							ostream & sout  -- the stream used for output
//
//				void print(console::ConsoleBuffer &out) const
//					prints using a console buffer
//						parameter
//							console::ConsoleBuffer &out
//								a console buffer
//
//
//    History Log:
//          	3/12/2017  PB  completed version 1.0
//-----------------------------------------------------------------------------
namespace sinkFleet {
	class CCell {
	public:
		CCell(unsigned short col = 0, unsigned short row = 0) :
			m_row(row), m_col(col), m_tested(false),
			currentShip(nullptr) { }

		unsigned short getRow( ) const { return m_row; }
		unsigned short getCol( ) const { return m_col; }

		void setTested( ) { m_tested = true; }
		bool isTested( ) const { return m_tested; }

        CShipInfo* getShip( ) {
            return currentShip;
        }

        void setShip(CShipInfo *ship) {
            currentShip = ship;
        }

		void print(std::ostream &out = std::cout) const {
			out << static_cast<char>('A' + m_col) << m_row;
		}
		void print(console::ConsoleBuffer &out) const {
			out << static_cast<char>('A' + m_col) << m_row;
		}

	private:
		unsigned short m_row;
		unsigned short m_col;
		bool m_tested;

		CShipInfo *currentShip;
	};

    inline std::ostream& operator<<(std::ostream &out, CCell &cell) {
        cell.print(out);
        return out;
    }

    inline console::ConsoleBuffer& operator<<(console::ConsoleBuffer &out, CCell &cell) {
        cell.print(out);
        return out;
    }
}

#endif // !CCELL_H
