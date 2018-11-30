//-------------------------------------------------------------------------------------
//File: CPlayer.cpp
//-------------------------------------------------------------------------------------
#include "CPlayer.h"

namespace sinkFleet {
	//---------------------------------------------------------------------------------
	// Function:	loadShip()
	// Title:	loadShip
	// Description:
	//		Sets the cells with the ship info
	// Programmer: Jordan Paladino, David Schmeling, Justin Nguyen and Kouassi Kouassi 
	// 
	// Date:	3/12/2017
	//
	// Version:	0.1
	// 
	// Environment: Hardware: i5
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Calls:		getCol()
	//				getRow()
	//				getShipType()
	//				getDirection()
	//				getSize()
	//				getCell()
	//				setCol()
	//				setRow()
	//				setPiecesLeft()
	//				setDirection()
	//				validLocation()
	// Called By:	loadGrid()
	//				update()
	//
	// Parameters:	CShipInfo &info
	//		size: char;		'S' or 'L'
	// 
	// Returns: bool -- 	true if sucessful
	//			false otherwise
	//
	// History Log:
	//      3/12/2017 JN completed v 0.1
	//---------------------------------------------------------------------------------
    bool CPlayer::loadShip(const CShipInfo &info) {
		bool success = false;
		if(validLocation(info.getCol( ), info.getRow( ), info.getShipType( ), info.getDirection( ))) {
			CShipInfo *hold = &m_ships[info.getShipType( )];
			hold->setCol(info.getCol( ));
			hold->setRow(info.getRow( ));
			hold->setPiecesLeft(shipSize[info.getShipType( )]);
			hold->setDirection(info.getDirection( ));
			for(short i = 0; i < info.getShipType( ).getSize( ); i++) {
				short dx = info.getCol( ) + i * (1 - info.getDirection( ));
				short dy = info.getRow( ) + i * info.getDirection( );
				setCell(dx, dy, info.getShipType( ));
			}
			success = true;
		}
        return success;
    }
	//---------------------------------------------------------------------------------
	// Function:	removeShip()
	// Title:	removeShip 
	// Description:
	//		Removes the ship from the location and the ship information
	// Programmer: Jordan Paladino, David Schmeling, Justin Nguyen and Kouassi Kouassi 
	// 
	// Date:	3/12/2017
	//
	// Version:	0.1
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Calls:		getCell()
	//				getShip()
	//				getShipType()
	//				getSize()
	//				getCol()
	//				getRow()
	//				getDirection()
	//				clearCell()
	//				setCol()
	//				setRow()
	//				setDirection()
	//				setPiecesLeft()
	//
	// Called By:	loadGrid()
	//				update()
	//
	// Parameters:	unsigned short x
	//				unsigned short y
	// 
	// Returns: bool -- 	true if the ship could be removed
	//			false otherwise
	//
	// History Log:
	//      3/12/2017 JN completed v 0.1
	//---------------------------------------------------------------------------------
    bool CPlayer::removeShip(unsigned short x, unsigned short y) {
		CCell cell = getCell(x, y);
		bool success = false;
		if(cell.getShip( ) != nullptr) {
			CShipInfo *info = cell.getShip( );
			for(short i = 0; i < info->getShipType( ).getSize( ); i++) {
				short dx = info->getCol( ) + i * (1 - info->getDirection( ));
				short dy = info->getRow( ) + i * info->getDirection( );
				clearCell(dx, dy);
			}
			info->setCol(USHRT_MAX);
			info->setRow(USHRT_MAX);
			info->setDirection(HORIZONTAL);
			info->setPiecesLeft(SHRT_MAX);
			success = true;
		}
        return success;
    }
	//---------------------------------------------------------------------------------
	// Function:	randomizeGrid()
	// Title:	RandomizeGrid
	// Description:
	//		Ships are placed at random locations instead of user input or by file
	// Programmer: Jordan Paldino, David Schmeling, Justin Nguyen, and Kouassi Kouassi 
	// 
	// Date:	3/12/2017
	//
	// Version:	0.1
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Calls: setRow()
	//		  setCol()
	//		  setPiecesLeft()
	//		  setDirection()
	//		  getSize()
	//
	// Called By: getCell()
	//			  validLocation()
	//
	// Parameters:	
	//	
	// 
	// Returns: void
	//
	// History Log:
	//      3/12/2017 JN completed v 0.1
	//---------------------------------------------------------------------------------
    void CPlayer::randomizeGrid( ) {
        setupShips( );
        short gridSize = BASE_GRID_SIZE + BASE_SCALE_SIZE * m_size;
		for(short i = 1; i < SHIP_SIZE - 2; i++) {
			Ship ship = static_cast<ship_type>(i);
			CDirection dir = static_cast<direction>(rand( ) % 2);
			unsigned short x = 0;
			unsigned short y = 0;
			do {
				x = rand( ) % gridSize - ship.getSize( ) * (1 - dir);
				y = rand( ) % gridSize - ship.getSize( ) * dir;
			} while(!validLocation(x, y, ship, dir));
			CShipInfo *info = &m_ships[i];
			info->setCol(x);
			info->setRow(y);
			info->setPiecesLeft(shipSize[i]);
			info->setDirection(dir);

			for(short i = 0; i < ship.getSize( ); i++) {
				short dx = x + i * (1 - dir);
				short dy = y + i * dir;
				setCell(dx, dy, ship);
			}
		}
		m_piecesLeft = SHIP_SIZE - 3;
	}
	//---------------------------------------------------------------------------------
	// Function:	setupShips()
	// Title:	setupShips
	// Description:
	//		Sets the ship info
	// Programmer: Jordan Paladino, David Schmeling, Justin Nguyen and Kouassi Kouassi 
	// 
	// Date:	3/12/2017
	//
	// Version:	0.1
	// 
	// Environment: Hardware: i5
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Calls: CShipInfo()
	//
	// Called By: randomizeGrid()
	//			  operator=()
	//
	// Parameters:
	// 
	// Returns: void
	//
	// History Log:
	//      3/12/2017 JN completed v 0.1
	//---------------------------------------------------------------------------------
    void CPlayer::setupShips( ) {
		for(short i = 0; i < SHIP_SIZE; i++) {
			m_ships[i] = CShipInfo(HORIZONTAL ,USHRT_MAX,
				USHRT_MAX, static_cast<ship_type>(i));
		}
		m_piecesLeft = SHIP_SIZE - 3;
	}
	//---------------------------------------------------------------------------------
	// Function:	clearGrid()
	// Title:	clearGrid
	// Description:
	//		Clears the grid of the ships and makes the grid empty
	// Programmer: Jordan Paladino, David Schmeling, Justin Nguyen and Kouassi Kouassi 
	// 
	// Date:	3/12/2017
	//
	// Version:	0.1
	// 
	// Environment: Hardware: i5
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Calls:		CCell(x,y)
	//
	// Called By:	update()
	//
	// Parameters:	gridSection section
	// 
	// Returns: void
	//			
	//
	// History Log:
	//      3/12/2017 JN and KK completed v 0.1
	//---------------------------------------------------------------------------------
	void CPlayer::clearGrid(gridSection section) {
        short gridSize = BASE_GRID_SIZE + BASE_SCALE_SIZE * m_size;
		for(short i = 0; i < gridSize * gridSize; i++) {
			short x = i % gridSize;
			short y = i / gridSize;
			m_gameGrid[i + gridSize *
                gridSize * section] = CCell(x, y);
		}
	}
	//---------------------------------------------------------------------------------
	// Function:	allocMem()
	// Title:		Allocate Memory
	// Description:
	//		allocates memory for  the grid
	// Programmer:	Jordan Paldino, David Schmeling, Justin Nguyen, and Kouassi Kouassi
	// modified by:	
	// 
	// Date:	3/12/2017
	//
	// Version:	1.01
	// 
	// Environment: Hardware: i3 
	//              Software: OS: Windows 7; 
	//              Compiles under Microsoft Visual C++ 2013
	//
	// Calls:		none
	//
	// Called By:	operator =
	//
	// Parameters:	none
	// 
	// Returns:	void
	//
	// History Log:
	//		3/12/2017 JN completed v 1.0
	//---------------------------------------------------------------------------------
    void CPlayer::allocMemory( ) {
        short gridSize = BASE_GRID_SIZE + BASE_SCALE_SIZE * m_size;
		m_gameGrid = new CCell[gridSize * gridSize * 2];
		for(short i = 0; i < gridSize * gridSize; i++) {
			short x = i % gridSize;
			short y = i / gridSize;
			m_gameGrid[i] = CCell(x, y);
		}
    }
	//---------------------------------------------------------------------------------
	// Function:	deleteMem()
	// Title:		Delete Memory
	// Description:
	//		Safely deletes memory for grids
	// Programmer: Jordan Paladino, David Schmeling, Justin Nguyen and Kouassi Kouassi
	// 
	// Date:	3/12/17
	//
	// Version:	1.0
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2013
	//
	// Calls:
	//
	// Called By:	operator =()
	//
	// Parameters:	None
	// 
	// Returns:	void
	//
	// History Log:
	//		3/12/2017 JN completed v 1.0
	//---------------------------------------------------------------------------------
    void CPlayer::deleteMemory( ) {
        short gridSize = BASE_GRID_SIZE + BASE_SCALE_SIZE * m_size;
		for(short i = 0; i < gridSize * gridSize; i++) {
			m_gameGrid[i].setShip(nullptr);
		}
        try {
		    delete[ ] m_gameGrid;
        } catch(std::exception e) {
            std::cerr << e.what( ) << std::endl;
        }
    }

	//---------------------------------------------------------------------------------
	// Function:	saveGrid()
	// Title:	Save Grid 
	// Description:
	//		Saves the ship grid to a file
	// Programmer: Justin Nguyen and Kouassi Kouassi
	// 
	// Date:	1/30/17
	//
	// Version: 1.0
	// 
	// Environment: Hardware: i5
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Output:	grid to specified file
	//
	// Calls:	getRow()
	//			getCol()
	//
	// Called By:	update()
	//
	// Parameters:	string &location
	// 
	// Returns:	bool good if sucessful, false if not
	//
	// History Log:
	//		3/12/2017 JN completed 1.0
	//     
	//---------------------------------------------------------------------------------
    bool CPlayer::saveGrid(const std::string &location) {
		// 0 - small
		// 1 - normal
		// 2 - large
		// 1{F,V,1,2}{S,H,5,3}
		std::string fileLoc = location;
		bool good = false;
		try {
			if(fileLoc.find(".save") == -1)
				fileLoc += ".save";
			std::ofstream outFile(location, std::ofstream::out);

			short shrtSize = m_size + 1;
			std::stringstream strOut;
			std::string outStr;
			strOut << shrtSize;

			for(int i = 1; i < SHIP_SIZE - 2; i++) {
				strOut << '{'
					<< static_cast<ship_type>(m_ships[i].getShipType( )) << ','
					<< static_cast<direction>(m_ships[i].getDirection( )) << ','
					<< m_ships[i].getCol( ) << ','
					<< m_ships[i].getRow( ) << '}';
			}
			strOut >> outStr;

			if(outFile) {
				outFile << outStr;
				outFile.close( );
				good = true;
			}
		} catch(std::exception e) {
			// TODO: handle exception
		}
		return good;
    }
	//---------------------------------------------------------------------------------
	// Function:	loadGrid()
	// Title:	loadGrid 
	// Description:
	//		Reads grid from a file and properly sets the ships
	// Programmer:	Jordan Paladino , Justin Nguyen, and Kouassi Kouassi, David Schmeling
	// modified by:	
	// 
	// Date:	3/12/2017
	//
	// Version:	0.5
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Input:	a string with the file name
	//
	// Output:	sets the ships onto the grid
	//
	// Calls: *info->getRow
	//        *info->getCol
	//        *info->setPiecesLeft
	//		   removeShips()
	// Called by: update()

	// Parameters:	
	//		string &location:	the name of the file to be opened for reading
	// 
	// Returns: bool -- 	true if the file is opened and read;
	//			false otherwise
	//
	// History Log: 
	//		3/12/17 JN completed v 1.0
	//---------------------------------------------------------------------------------
    bool CPlayer::loadGrid(const std::string &location) {
		std::string fileLoc = location;
        std::ifstream inFile;
		std::string inStr;

		bool good = false;
		try {
			inFile.open(fileLoc);
			if(!inFile && fileLoc.find(".save") == -1) {
				fileLoc += ".save";
				inFile.open(fileLoc);
			}

			if(inFile) {
				inFile >> std::skipws >> inStr;
				inFile.close( );
			
				for(short i = 1; i < SHIP_SIZE - 2; i++) {
					CShipInfo *info = &m_ships[i];
					if(info->getCol( ) != USHRT_MAX && info->getRow( ) != USHRT_MAX) {
						bool rem = removeShip(info->getCol( ), info->getRow( ));
						if(!rem) {
							info->setPiecesLeft(-1);
							info->setCol(USHRT_MAX);
							info->setRow(USHRT_MAX);
						}
					}
				}

				short size = 0;
				{
					std::stringstream strStream;
					strStream << inStr;
					strStream >> size;
				}
				if(m_size == size - 1) {
					short currLoc = -1;
					short end = -1;
					short shipId = -1;
					short data = -1;
					m_size = static_cast<gameSize>(size - 1);
					do {
						currLoc = static_cast<short>(inStr.find('{', currLoc + 1));
						end = static_cast<short>(inStr.find('}', currLoc + 1));
						std::stringstream strStream;
						strStream << inStr.substr(currLoc + 1, end - currLoc - 1);
						strStream >> data;
						shipId = data;
						if(shipId > 0 && shipId < 6) {
							CShipInfo *hold = &m_ships[shipId];
							if(hold->getCol( ) == USHRT_MAX 
								&& hold->getRow( ) == USHRT_MAX) {
                                CShipInfo info;
								strStream.get();
								info.setShipType(static_cast<ship_type>(shipId));
								strStream >> data;
								strStream.get();
								info.setDirection(static_cast<direction>(data));
								strStream >> data;
								strStream.get();
								info.setCol(data);
								strStream >> data;
								strStream.get();
								info.setRow(data);
								info.setPiecesLeft(shipSize[shipId]);
                                loadShip(info);
							}
						}
					} while(currLoc != -1);
					good = true;
				}
			}
		}catch(std::exception e) {
			// TODO: add exception handeling
		}
		return good;
    }
	//---------------------------------------------------------------------------------
	// Function:	withinBounds()
	// Title:	withinBounds 
	// Description:
	//		Checks to see if the ship goes off the grid
	// Programmer: Jordan Paladino, David Schmeling, Justin Nguyen and Kouassi Kouassi 
	// 
	// Date:	3/12/2017
	//
	// Version:	0.1
	// 
	// Environment: Hardware: i5
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Calls:		getSize()
	//
	// Called By:	validLocation()
	//				update()
	//
	// Parameters:	short x
	//				short y
	//				Ship &ship
	//				CDirection &direction
	// 
	// Returns: bool -- 	true if the ship would not go off the edge
	//			false otherwise
	//
	// History Log:
	//     3/12/2017 JN completed v 0.1
	//---------------------------------------------------------------------------------
    bool CPlayer::withinBounds(short x, short y, const Ship &ship, const CDirection &direction) {
		bool inbounds = false;
		short size = BASE_GRID_SIZE + BASE_SCALE_SIZE * m_size;
        if(ship == NOSHIP || ship == HIT || ship == MISSED) {
			if (x >= 0 && x < size && y >= 0 && y < size) {
				inbounds = true;
			}
		} else {
			short s = ship.getSize() - 1;
			if(x >= 0 && x < size - s * (1 - direction)
				&& y >= 0 && y < size - s * direction) {
				inbounds = true;
			}
		}
        return inbounds;
    }
	//---------------------------------------------------------------------------------
	// Function:	validLocation()
	// Title:	Valid Location 
	// Description:
	//		Can the ship legitimately go there?
	// Programmer: Jordan Paladino, David Schmeling, Justin Nguyen and Kouassi Kouassi 
	// 
	// Date:	3/12/2017
	//
	// Version:	0.1
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Calls:		getSize()
	//				getCell()
	//				getShip()
	//				withinBounds()
	//
	// Called By:	loadShip()
	//				randomizeGrid()
	//				update()
	//
	// Parameters:  short x
	//				short y
	//				Ship &ship
	//				CDirection &direction
	// 
	// Returns: bool -- 	true if the ship can be placed
	//			false otherwise
	//
	// History Log:
	//      1/30/17 JN completed v 0.1
	//---------------------------------------------------------------------------------
    bool CPlayer::validLocation(short x, short y, const Ship &ship, const CDirection &direction) {
		bool valid = false;
		if(withinBounds(x, y, ship, direction)
			&& !(ship == NOSHIP || ship == HIT || ship == MISSED)) {
			valid = true;
			for(int i = 0; i < ship.getSize( ) && valid; i++) {
				short dx = x + i * (1 - direction);
				short dy = y + i * direction;
				CCell cell = getCell(dx, dy);
				if(cell.getShip( ) != nullptr) {
					valid = false;
				}
			}
		}
        return valid;
    }
	//---------------------------------------------------------------------------------
	// Function:	getCell()
	// Title:	getCell
	// Description:
	//		Determines whether the cell is inbounds and returns the coordinates of the cell
	// Programmer:	Jordan Paladino , Justin Nguyen, and Kouassi Kouassi, David Schmeling
	// modified by:	
	// 
	// Date:	3/12/2017
	//
	// Version:	0.5
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Input:	none
	// Output:	none
	//
	// Calls: none
	// Called by: setCell()
	//			  clearCell()
	//			  hitCell()
	// Parameters:	
	//		unsigned short x
	//			unsigned short y
	//			gridSection section
	//
	// 
	// Returns: bool -- CCell location
	//
	// History Log: 
	//		3/12/17 JN completed v 1.0
	//---------------------------------------------------------------------------------
    CCell& CPlayer::getCell(unsigned short x, unsigned short y, gridSection section) {
        if(!withinBounds(x, y))
            throw std::out_of_range("Coordinates were out of bounds");
        short size = BASE_GRID_SIZE + BASE_SCALE_SIZE * m_size;
        return m_gameGrid[x + y * size + size * size * section];
    }
	//---------------------------------------------------------------------------------
	// Function:	getCell()
	// Title:	getCell
	// Description:
	//		sets the ship at the cell location
	// Programmer:	Jordan Paladino , Justin Nguyen, and Kouassi Kouassi, David Schmeling
	// modified by:	
	// 
	// Date:	3/12/2017
	//
	// Version:	0.5
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Input:	none
	//
	// Output:	none
	//
	// Calls: none
	// Called by: randomizeGrid()
	//			  loadShip()
	// Parameters:	
	//		unsigned short x
	//			unsigned short y
	//			gridSection section
	//			Ship &ship
	//
	// Returns: void
	//
	// History Log: 
	//		3/12/17 JN completed v 1.0
	//---------------------------------------------------------------------------------
    void CPlayer::setCell(unsigned short x, unsigned short y, const Ship &ship, gridSection section) {
        CCell &cell = getCell(x, y, section);
        cell.setShip(&m_ships[ship]);
    }
	//---------------------------------------------------------------------------------
	// Function:	clearCell()
	// Title:	clearCell
	// Description:
	//		clears the ship at the cell location
	// Programmer:	Jordan Paladino , Justin Nguyen, and Kouassi Kouassi, David Schmeling
	// modified by:	
	// 
	// Date:	3/12/2017
	//
	// Version:	0.5
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Input:	none
	//
	// Output:	none
	//
	// Calls: getCell()
	//		  setShip()
	// Called by: removeShip()
	//			  clearCell()
	// Parameters:	
	//		unsigned short x
	//			unsigned short y
	//			gridSection section
	//
	// Returns: void
	//
	// History Log: 
	//		3/12/17 JN completed v 1.0
	//---------------------------------------------------------------------------------
    void CPlayer::clearCell(unsigned short x, unsigned short y, gridSection section) {
        CCell &cell = getCell(x, y, section);
        cell.setShip(nullptr);
    }
	//---------------------------------------------------------------------------------
	// Function:	hitCell()
	// Title:	hitCell
	// Description:
	//		determines whether the shot was hit or missed
	// Programmer:	Jordan Paladino , Justin Nguyen, and Kouassi Kouassi, David Schmeling
	// modified by:	
	// 
	// Date:	3/12/2017
	//
	// Version:	0.5
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Input:	none
	//
	// Output:	none
	//
	// Calls: getShipType()
	//		  getShip()
	//		  isSunk()
	//		  getCell()
	//		  
	// Called by: update()
	// Parameters:	
	//		unsigned short x
	//			unsigned short y
	//	
	//
	// Returns: hitState state
	//
	// History Log: 
	//		3/12/17 JN completed v 1.0
	//---------------------------------------------------------------------------------
    hitState CPlayer::hitCell(unsigned short x, unsigned short y) {
        CCell &cell = getCell(x, y);
        hitState state = MISS;
        if(cell.getShip( ) != nullptr) {
            if(cell.getShip( )->getShipType( ) != NOSHIP
                && cell.getShip( )->getShipType( ) != MISSED
                && cell.getShip( )->getShipType( ) != HIT) {
                state = FOUND;
                if((--*cell.getShip( )).isSunk( )) {
                    state = SUNK;
                    m_piecesLeft--;
                }
            }
        }
        return state;
    }
	//---------------------------------------------------------------------------------
	// Function:	operator=()
	// Title:	operator=()
	// Description:
	//		Creates a copy of the player
	// Programmer:	Jordan Paladino , Justin Nguyen, and Kouassi Kouassi, David Schmeling
	// modified by:	
	// 
	// Date:	3/12/2017
	//
	// Version:	0.5
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Input:	none
	//
	// Output:	none
	//
	// Calls: deleteMemory()
	//		  allocMemory()
	//		  setupShips()
	//	      setCol()
	//		  setRow()
	//		  setDirection()
	//		  setPiecesLeft()
	//		  getShipType()
	// Called by: update()
	// Parameters:	
	//		unsigned short x
	//			unsigned short y
	//			gridSection section
	//
	// Returns: A copy of the CPlayer passed in
	//
	// History Log: 
	//		3/12/17 JN completed v 1.0
	//---------------------------------------------------------------------------------
    CPlayer& CPlayer::operator=(const CPlayer &player) {
        if(m_constructed)
            deleteMemory( );
        m_size = player.m_size;
        m_whichPlayer = player.m_whichPlayer;
        short gridSize = BASE_GRID_SIZE + BASE_SCALE_SIZE * m_size;
        allocMemory( );
        setupShips( );
        for(int i = 0; i < SHIP_SIZE; i++) {
            CShipInfo *ship = &m_ships[i];
            CShipInfo hold = player.m_ships[i];
            ship->setCol(hold.getCol( ));
            ship->setRow(hold.getRow( ));
            ship->setDirection(hold.getDirection( ));
            ship->setPiecesLeft(hold.getPiecesLeft( ));
        }
        for(int i = 0; i < gridSize * gridSize; i++) {
            CShipInfo *ship = m_gameGrid[i].getShip( );
            if(ship != nullptr) {
                if(ship->getShipType( ) != NOSHIP) {
                    short x = i % gridSize;
                    short y = i / gridSize;
                    setCell(x, y, ship->getShipType( ));
                }
            }
        }
        m_piecesLeft = player.m_piecesLeft;
        return *this;
    }

    const unsigned char DOUBLE_VERT_LINE = 0xba;
    const unsigned char DOUBLE_HORI_LINE = 0xcd;
    const unsigned char DOUBLE_CROSS = 0xce;
    const unsigned char LEFT_CROSS_EDGE = 0xcc;
    const unsigned char TOP_CROSS_EDGE = 0xcb;

    const unsigned char CROSS = 0xc5;
    const unsigned char VERT_LINE = 0xb3;
    const unsigned char HORI_LINE = 0xc4;
	//---------------------------------------------------------------------------------
	// Function:	printGrid()
	// Title:	Print Ship 
	// Description:
	//		Creates the grid and prints it out
	// Programmer: Jordan Paladino , Justin Nguyen, and Kouassi Kouassi, David Schmeling
	// 
	// Date:	3/12/2017
	//
	// Version:	0.1
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Output:	a grid to sout
	//
	// Calls:	getCell()
	//			getShip()
	//
	// Called By:	update()
	//
	// Parameters:	ConsoleBuffer & out
	//				int x
	//				int y
	//				gridSection section
	// 
	// Returns:	void
	//
	// History Log:
	//			JN updated v0.1 3/12/2017
	//-------------------------------------------------------------------
    void CPlayer::printGrid(console::ConsoleBuffer & out, int x, int y, gridSection section) {
        printGrid(out, x, y, m_size);
        int size = BASE_GRID_SIZE + BASE_SCALE_SIZE * m_size;
        for(int i = 0; i < size * size; i++) {
            short dx = i % size;
            short dy = i / size;
            CShipInfo *info = getCell(dx, dy, section).getShip( );
            if(info != nullptr) {
                out(dx * 4 + x + 5, dy * 2 + y + 2);
                out << info->getShipType( );
            }
        }
    }
	//---------------------------------------------------------------------------------
	// Function:	printGrid()
	// Title:	Print Ship 
	// Description:
	//		Creates the grid and prints it out
	// Programmer: Jordan Paladino , Justin Nguyen, and Kouassi Kouassi, David Schmeling
	// 
	// Date:	3/12/2017
	//
	// Version:	0.1
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Output:	a grid to sout
	//
	// Calls:	getCell()
	//			getShip()
	//
	// Called By:	update()
	//
	// Parameters:	ConsoleBuffer & out
	//				int x
	//				int y
	//				gameSize size
	// 
	// Returns:	void
	//
	// History Log:
	//			JN updated v0.1 3/12/2017
	//-------------------------------------------------------------------
    void CPlayer::printGrid(console::ConsoleBuffer & out, int x, int y, gameSize size) {
        int s = BASE_GRID_SIZE + BASE_SCALE_SIZE * size;

        char k = 0;
        int l = 1;
        out.setCursorPos(x, y);
        for(short i = 0; i < s * 2 + 2; i++) {
            for(short j = 0; j < s * 2 + 2; j++) {
                short j2 = j % 2;
                short i2 = i % 2;

                if(i == 0 && j == 0)
                    out << "   ";
                else if(i == 0)
                    if(j2 == 1)
                        out << DOUBLE_VERT_LINE;
                    else
                        out << " " << static_cast<char>('A' + k++) << " ";
                else if(j == 0)
                    if(i2 == 1)
                        out << DOUBLE_HORI_LINE << DOUBLE_HORI_LINE << DOUBLE_HORI_LINE;
                    else
                        out << " " << console::setS(2) << l++;
                else if(j2 == 1 && i2 == 1)
                    if(i == 1 || j == 1)
                        out << DOUBLE_CROSS;
                    else
                        out << CROSS;
                else if(j2 == 0 && i2 == 0)
                    out << "   ";
                else if(j > 0 && i > 0)
                    if(j == 1)
                        out << DOUBLE_VERT_LINE;
                    else if(i == 1)
                        out << DOUBLE_HORI_LINE << DOUBLE_HORI_LINE << DOUBLE_HORI_LINE;
                    else if(i2 == 1 && j2 == 0)
                        out << HORI_LINE << HORI_LINE << HORI_LINE;
                    else
                        out << VERT_LINE;
                else
                    out << " ";

            }
            out.setCursorPos(x, ++y);
        }
    }
	//---------------------------------------------------------------------------------
	// Function:	printGrid()
	// Title:	Print Ship 
	// Description:
	//		Creates the grid and prints it out
	// Programmer: Jordan Paladino , Justin Nguyen, and Kouassi Kouassi, David Schmeling
	// 
	// Date:	3/12/2017
	//
	// Version:	0.1
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Output:	a grid to sout
	//
	// Calls:	getCell()
	//			getShip()
	//
	// Called By:	update()
	//
	// Parameters:	gridSection section
	//				ostream& out
	// 
	// Returns:	void
	//
	// History Log: 
	//			JN updated v0.1 3/12/2017
	//-------------------------------------------------------------------
    void CPlayer::printGrid(gridSection section, std::ostream & out) {
        int s = BASE_GRID_SIZE + BASE_SCALE_SIZE * m_size;

        char k = 0;
        int l = 1;
        for(short i = 0; i < s * 2 + 2; i++) {
            for(short j = 0; j < s * 2 + 2; j++) {
                short j2 = j % 2;
                short i2 = i % 2;

                if(i == 0 && j == 0)
                    out << "   ";
                else if(i == 0)
                    if(j2 == 1)
                        out << DOUBLE_VERT_LINE;
                    else
                        out << " " << static_cast<char>('A' + k++) << " ";
                else if(j == 0)
                    if(i2 == 1)
                        out << DOUBLE_HORI_LINE << DOUBLE_HORI_LINE << DOUBLE_HORI_LINE;
                    else
                        out << " " << std::setw(2) << l++;
                else if(j2 == 1 && i2 == 1)
                    if(i == 1 || j == 1)
                        out << DOUBLE_CROSS;
                    else
                        out << CROSS;
                else if(j2 == 0 && i2 == 0) {
                    CCell cell = getCell(j / 2 - 1, i / 2 - 1, section);
                    if(cell.getShip( ) != nullptr) {
                        out << " " << (*cell.getShip( )) << " ";
                    } else {
                        out << "   ";
                    }
                } else if(j > 0 && i > 0)
                    if(j == 1)
                        out << DOUBLE_VERT_LINE;
                    else if(i == 1)
                        out << DOUBLE_HORI_LINE << DOUBLE_HORI_LINE << DOUBLE_HORI_LINE;
                    else if(i2 == 1 && j2 == 0)
                        out << HORI_LINE << HORI_LINE << HORI_LINE;
                    else
                        out << VERT_LINE;
                else
                    out << " ";

            }
            out << std::endl;
        }
    }
	//---------------------------------------------------------------------------------
	// Function:	printGrid()
	// Title:	Print Ship 
	// Description:
	//		Creates the grid and prints it out
	// Programmer: Jordan Paladino , Justin Nguyen, and Kouassi Kouassi, David Schmeling
	// 
	// Date:	3/12/2017
	//
	// Version:	0.1
	// 
	// Environment: Hardware: i5 
	//              Software: OS: Windows 10; 
	//              Compiles under Microsoft Visual C++ 2015
	//
	// Output:	a grid to sout
	//
	// Calls:	getCell()
	//			getShip()
	//
	// Called By:	update()
	//
	// Parameters:	gameSize size
	//				ostream& out
	// 
	// Returns:	void
	//
	// History Log:
	//			JN updated v0.1 3/12/2017
	//-------------------------------------------------------------------
    void CPlayer::printGrid(gameSize size, std::ostream & out) {
        int s = BASE_GRID_SIZE + BASE_SCALE_SIZE * size;

        char k = 0;
        int l = 1;
        for(short i = 0; i < s * 2 + 2; i++) {
            for(short j = 0; j < s * 2 + 2; j++) {
                short j2 = j % 2;
                short i2 = i % 2;

                if(i == 0 && j == 0)
                    out << "   ";
                else if(i == 0)
                    if(j2 == 1)
                        out << DOUBLE_VERT_LINE;
                    else
                        out << " " << static_cast<char>('A' + k++) << " ";
                else if(j == 0)
                    if(i2 == 1)
                        out << DOUBLE_HORI_LINE << DOUBLE_HORI_LINE << DOUBLE_HORI_LINE;
                    else
                        out << " " << std::setw(2) << l++;
                else if(j2 == 1 && i2 == 1)
                    if(i == 1 || j == 1)
                        out << DOUBLE_CROSS;
                    else
                        out << CROSS;
                else if(j2 == 0 && i2 == 0)
                    out << "   ";
                else if(j > 0 && i > 0)
                    if(j == 1)
                        out << DOUBLE_VERT_LINE;
                    else if(i == 1)
                        out << DOUBLE_HORI_LINE << DOUBLE_HORI_LINE << DOUBLE_HORI_LINE;
                    else if(i2 == 1 && j2 == 0)
                        out << HORI_LINE << HORI_LINE << HORI_LINE;
                    else
                        out << VERT_LINE;
                else
                    out << " ";

            }
            out << std::endl;
        }
    }
}