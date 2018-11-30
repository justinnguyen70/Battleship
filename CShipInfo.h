//-----------------------------------------------------------------------------
//	File:           CShipInfo.h
//
//	class:		CShipInfo
//-----------------------------------------------------------------------------
#ifndef CSHIPINFO_H
#define CSHIPINFO_H

#include "console.h"
#include "Ship.h"
#include "CDirection.h"
//-----------------------------------------------------------------------------
//	Title:          CShipInfo  Class
//
//	Description:    This file contains the class definition for CShipInfo 
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
//				CDirection m_orientation: orientation of the ship
//				unsigned short m_row : actual row
//				unsigned short m_col : actual column
//				Ship m_ship : actual ship
//				short m_piecesLeft : actual pieces left
//
//	Methods:
//	
//		inline:
//			
//			CShipInfo(CDirection direction = horizontal, unsigned short col = 0, 
//					unsigned short row = 0, Ship ship = ship_type(0)) :
//					m_orientation(direction), m_row(row), m_col(col), 
//					m_ship(ship), m_piecesLeft(shipSize[static_cast<ship_type>(m_ship)])
//				initialize m_orientation, m_row, m_col, m_ship, and m_piecesLeft
//				paramaters:
//					CDirection direction  : the direction of the ship
//					unsigned short col : the column
//					unsigned short col : the row 
//					Ship ship: a ship
//
//				CDirection getDirection( ) const --accessor
//					return m_orientation
//
//				unsigned short getRow() const  -- accessor
//					return m_row
//
//				unsigned short getCol() const  -- accessor
//					return m_col
//
//				Ship getShipType() const	--	accessor
//				    return m_ship
//
//				short getPiecesLeft() const -- accessor
//					return m_piecesLeft
//
//				void setDirection(CDirection dir) 
//					set m_orientation to dir 
//						parameter
//							CDirection dir: CDirection object
//
//				void setCol(unsigned short col) -- mutator
//					set m_col to col
//						parameter
//							unsigned short col--the column
//
//				void setRow(unsigned short row) -- mutator
//					set m_row to row
//						parameter
//							unsigned short row-- the row
//
//				void setShipType(Ship ship) -- mutator
//					set m_ship to ship
//						paremeter
//							Ship ship-- ship object
//
//				void setPiecesLeft(short count) -- mutator
//					set m_piecesLeft to count
//						parameter 
//							short count-- count pieces left
//							
//
//				CShipInfo& operator--()
//					decrease m_piecesLeft and return a reference to CShipInfo
//
//				CShipInfo operator--(int unused)
//					calls operator-- and return a CShipInfo object
//						parameter
//							int unused--
//
//			void print(std::ostream &out = std::cout) const
//				prints out  m_ship, m_orientation, m_row, m_col, and m_piecesLeft
//							to the std output stream
//						parameter
//							std::ostream & sout  -- the stream used for output
//
//			bool isSunk( ) const
//				return the state of m_piecesLeft <= 0
//
//			void print(console::ConsoleBuffer &out) const
//				prints out  m_ship, m_orientation, m_row, m_col, and m_piecesLeft
//							to the console buffer stream
//						parameter
//							console::ConsoleBuffer &out
//								a console buffer
//
//			 inline std::ostream& operator<<(std::ostream &out, const CShipInfo &ship)
//				output stream to the std buffer
//					parameter
//						std::ostream& out -- the std buffer
//						const CShipInfo &ship -- a reference to  CShipInfo
//
//			inline console::ConsoleBuffer& operator<<(console::ConsoleBuffer &out, const CShipInfo &ship)
//					output stream to the console buffer
//					parameter
//						console::ConsoleBuffer &out-- the console Buffer buffer
//						const CShipInfo &ship -- a reference to  CShipInfo
//
//    History Log:
//          	3/12/2017  PB  completed version 1.0
//-----------------------------------------------------------------------------
namespace sinkFleet {

	class CShipInfo {
	public:
		CShipInfo(CDirection direction = horizontal, unsigned short col = 0, unsigned short row = 0, Ship ship = ship_type(0)) :
			m_orientation(direction), m_row(row), m_col(col), m_ship(ship), m_piecesLeft(shipSize[static_cast<ship_type>(m_ship)]) { }
	
		void print(std::ostream &out = std::cout) const {
			out << m_ship << ", located " << m_orientation
				<< "ly at " << static_cast<char>('A' + m_row)
				<< ", " << m_col << " Count of " << m_piecesLeft;
		}
		void print(console::ConsoleBuffer &out) const {
			out << m_ship << ", located " << m_orientation
				<< "ly at " << static_cast<char>('A' + m_row)
				<< ", " << m_col << " Count of " << m_piecesLeft;
		}
	
		CDirection getDirection( ) const { return m_orientation; }
		unsigned short getRow( ) const { return m_row; }
		unsigned short getCol( ) const { return m_col; }
		Ship getShipType( ) const { return m_ship; }
		short getPiecesLeft( ) const { return m_piecesLeft; }

		void setDirection(CDirection dir) { m_orientation = dir; }
		void setCol(unsigned short col) { m_col = col; }
		void setRow(unsigned short row) { m_row = row; }
		void setShipType(Ship ship) { m_ship = ship; }
		void setPiecesLeft(short count) { m_piecesLeft = count; }

		CShipInfo& operator--() {
			m_piecesLeft--;
			return *this;
		}
		CShipInfo operator--(int unused) {
			CShipInfo info = (*this);
			operator--();
			return info;
		}

		bool isSunk( ) const {
			return m_piecesLeft <= 0;
		}

	private:
		CDirection m_orientation;
		unsigned short m_row;
		unsigned short m_col;
		Ship m_ship;
		short m_piecesLeft;
	};

	inline std::ostream& operator<<(std::ostream &out, const CShipInfo &ship) {
		ship.print(out);
		return out;
	}

	inline console::ConsoleBuffer& operator<<(console::ConsoleBuffer &out, const CShipInfo &ship) {
		ship.print(out);
		return out;
	}

}

#endif // !CSHIPINFO_H
