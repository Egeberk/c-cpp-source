#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Piece.h"
using namespace std;
class Board
{
private:

    Board();

	string map[8][8];

    vector<Piece> whitesPieces;

    vector<Piece> blackPieces;

    // Coordinate additions for Bishop's position
    Point bishopControlPoints[8]; 
	
    // Reads map from file
    void readBoardFile(const string fileName)
    {
        ifstream in(fileName);

        // Read 8 to 8 map
        if (in.is_open())
        {
            for (int i = 0; i < 8; ++i)
            {
                for (int j = 0; j < 8; ++j)
                    in >> map[i][j];
            }
        }
        else
        {
            cout <<endl << "ERROR: FILE NOT FOUND !";

            exit(EXIT_FAILURE);
        }

        in.close();
    }

    // Converts map into piece information
    void initPieces()
    {
        Piece p;

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                // Look for black pieces
                if (map[i][j] == "ks")
                {
                    p.setIdent("ks");
                    p.setValue(5);
                    p.setCoord(i, j);
                    blackPieces.push_back(p);
                }
                else if (map[i][j] == "as")
                {
                    p.setIdent("as");
                    p.setValue(3);
                    p.setCoord(i, j);
                    blackPieces.push_back(p);
                }
                else if (map[i][j] == "fs")
                {
                    p.setIdent("fs");
                    p.setValue(3);
                    p.setCoord(i, j);
                    blackPieces.push_back(p);
                }
                else if (map[i][j] == "vs")
                {
                    p.setIdent("vs");
                    p.setValue(9);
                    p.setCoord(i, j);
                    blackPieces.push_back(p);
                }
                else if (map[i][j] == "ss")
                {
                    p.setIdent("ss");
                    p.setValue(100);
                    p.setCoord(i, j);
                    blackPieces.push_back(p);
                }

                else if (map[i][j] == "ps")
                {
                    p.setIdent("ps");
                    p.setValue(1);
                    p.setCoord(i, j);
                    blackPieces.push_back(p);
                }
                // Look for white pieces
                else if (map[i][j] == "kb")
                {
                    p.setIdent("kb");
                    p.setValue(5);
                    p.setCoord(i, j);
                    whitesPieces.push_back(p);
                }
                else if (map[i][j] == "ab")
                {
                    p.setIdent("ab");
                    p.setValue(3);
                    p.setCoord(i, j);
                    whitesPieces.push_back(p);
                }
                else if (map[i][j] == "fb")
                {
                    p.setIdent("fb");
                    p.setValue(3);
                    p.setCoord(i, j);
                    whitesPieces.push_back(p);
                }
                else if (map[i][j] == "vb")
                {
                    p.setIdent("vb");
                    p.setValue(9);
                    p.setCoord(i, j);
                    whitesPieces.push_back(p);
                }
                else if (map[i][j] == "sb")
                {
                    p.setIdent("sb");
                    p.setValue(100);
                    p.setCoord(i, j);
                    whitesPieces.push_back(p);
                }
                else if (map[i][j] == "pb")
                {
                    p.setIdent("pb");
                    p.setValue(1);
                    p.setCoord(i, j);
                    whitesPieces.push_back(p);
                }
            }
        }

        // Init Bishop Control Points Array
        bishopControlPoints[0] = Point(-2, 1);
        bishopControlPoints[1] = Point(-1, 2);
        bishopControlPoints[2] = Point(1, 2);
        bishopControlPoints[3] = Point(2, 1);
        bishopControlPoints[4] = Point(2, -1);
        bishopControlPoints[5] = Point(1, -2);
        bishopControlPoints[6] = Point(-1, -2);
        bishopControlPoints[7] = Point(-2, -1);
    }

    // Checks if the given coordinate is out of the board
    bool isOutOfBounds(Point p)
    {
        if ((p.getRow() > 7) || (p.getRow() < 0))
            return true;
        else if ((p.getCol() > 7) || (p.getCol() < 0))
            return true;

        return false;
    }

    // Finds all threatened pieces and updates their data
    void findThreatened()
    {
        /*Find pieces threatened by whites*/ 

        for (int i = 0; i < whitesPieces.size(); i++)
        {
            // If a bishop is found, start identify the ones threatened by it
            if (whitesPieces[i].getIdent() == "ab")
            {
                Point bishopCoordiante = whitesPieces[i].getCoord();

                // Bishop patrols surroundings
                for (int j = 0; j < 8; j++)
                {
                    Point surrounding = bishopCoordiante + bishopControlPoints[j];

                    if (!isOutOfBounds(surrounding))
                    {
                        // Find threatened black piece
                        for (int k = 0; k < blackPieces.size(); k++)
                        {
                            if (blackPieces[k].getCoord() == surrounding)
                            {
                                blackPieces[k].setThreatened(true);
                                break;
                            }   
                        }
                    }

                    // Else: Bishop is looking out of the map, do nothing
                }
            }
            // If a knight is found, start identify the ones threatened by it
            else if (whitesPieces[i].getIdent() == "fb")
            {
                Point tempKnightCoordiante = whitesPieces[i].getCoord();

                bool knightFoundPiece = false;

                // Knight checks up right diagonal
                while (!isOutOfBounds(tempKnightCoordiante) && !knightFoundPiece)
                {
                    // Find threatened black piece
                    for (int k = 0; k < blackPieces.size(); k++)
                    {
                        if (blackPieces[k].getCoord() == (tempKnightCoordiante + Point(-1, 1)))
                        {
                            blackPieces[k].setThreatened(true);

                            knightFoundPiece = true;

                            break;
                        }
                    }

                    // Check if a friendly piece is blocking the patrol path
                    for (int k = 0; k < whitesPieces.size(); k++)
                    {
                        if (whitesPieces[k].getCoord() == (tempKnightCoordiante + Point(-1, 1)))
                        {
                            knightFoundPiece = true;

                            break;
                        }
                    }

                    tempKnightCoordiante = tempKnightCoordiante + Point(-1, 1);   
                }

                tempKnightCoordiante = whitesPieces[i].getCoord();

                knightFoundPiece = false;

                // Knight checks down right diagonal
                while (!isOutOfBounds(tempKnightCoordiante) && !knightFoundPiece)
                {
                    // Find threatened black piece
                    for (int k = 0; k < blackPieces.size(); k++)
                    {
                        if (blackPieces[k].getCoord() == (tempKnightCoordiante + Point(1, 1)))
                        {
                            blackPieces[k].setThreatened(true);

                            knightFoundPiece = true;

                            break;
                        }
                    }

                    // Check if a friendly piece is blocking the patrol path
                    for (int k = 0; k < whitesPieces.size(); k++)
                    {
                        if (whitesPieces[k].getCoord() == (tempKnightCoordiante + Point(1, 1)))
                        {
                            knightFoundPiece = true;

                            break;
                        }
                    }

                    tempKnightCoordiante = tempKnightCoordiante + Point(1, 1);
                }

                tempKnightCoordiante = whitesPieces[i].getCoord();

                knightFoundPiece = false;

                // Knight checks up left diagonal
                while (!isOutOfBounds(tempKnightCoordiante) && !knightFoundPiece)
                {
                    // Find threatened black piece
                    for (int k = 0; k < blackPieces.size(); k++)
                    {
                        if (blackPieces[k].getCoord() == (tempKnightCoordiante + Point(-1, -1)))
                        {
                            blackPieces[k].setThreatened(true);

                            knightFoundPiece = true;

                            break;
                        }
                    }

                    // Check if a friendly piece is blocking the patrol path
                    for (int k = 0; k < whitesPieces.size(); k++)
                    {
                        if (whitesPieces[k].getCoord() == (tempKnightCoordiante + Point(-1, -1)))
                        {
                            knightFoundPiece = true;

                            break;
                        }
                    }

                    tempKnightCoordiante = tempKnightCoordiante + Point(-1, -1);
                }

                // Knight checks down left diagonal
                while (!isOutOfBounds(tempKnightCoordiante) && !knightFoundPiece)
                {
                    // Find threatened black piece
                    for (int k = 0; k < blackPieces.size(); k++)
                    {
                        if (blackPieces[k].getCoord() == (tempKnightCoordiante + Point(1, -1)))
                        {
                            blackPieces[k].setThreatened(true);

                            knightFoundPiece = true;

                            break;
                        }
                    }

                    // Check if a friendly piece is blocking the patrol path
                    for (int k = 0; k < whitesPieces.size(); k++)
                    {
                        if (whitesPieces[k].getCoord() == (tempKnightCoordiante + Point(1, -1)))
                        {
                            knightFoundPiece = true;

                            break;
                        }
                    }

                    tempKnightCoordiante = tempKnightCoordiante + Point(1, -1);
                }
            }
        } // Control for white bishops and knights end
        
        /*Find pieces threatened by blacks*/

        for (int i = 0; i < blackPieces.size(); i++)
        {
            // If a bishop is found, start identify the ones threatened by it
            if (blackPieces[i].getIdent() == "as")
            {
                Point bishopCoordiante = blackPieces[i].getCoord();

                // Bishop patrols surroundings
                for (int j = 0; j < 8; j++)
                {
                    Point surrounding = bishopCoordiante + bishopControlPoints[j];

                    if (!isOutOfBounds(surrounding))
                    {
                        // Find threatened white piece
                        for (int k = 0; k < whitesPieces.size(); k++)
                        {
                            if (whitesPieces[k].getCoord() == surrounding)
                            {
                                whitesPieces[k].setThreatened(true);
                                break;
                            }
                        }
                    }

                    // Else: Bishop is looking out of the map, do nothing
                }
            }
            // If a knight is found, start identify the ones threatened by it
            else if (blackPieces[i].getIdent() == "fs")
            {
                Point tempKnightCoordiante = blackPieces[i].getCoord();

                bool knightFoundPiece = false;

                // Knight checks up right diagonal
                while (!isOutOfBounds(tempKnightCoordiante) && !knightFoundPiece)
                {
                    // Find threatened white piece
                    for (int k = 0; k < whitesPieces.size(); k++)
                    {
                        if (whitesPieces[k].getCoord() == (tempKnightCoordiante + Point(-1, 1)))
                        {
                            whitesPieces[k].setThreatened(true);

                            knightFoundPiece = true;

                            break;
                        }
                    }

                    // Check if a friendly piece is blocking the patrol path
                    for (int k = 0; k < blackPieces.size(); k++)
                    {
                        if (blackPieces[k].getCoord() == (tempKnightCoordiante + Point(-1, 1)))
                        {
                            knightFoundPiece = true;

                            break;
                        }
                    }

                    tempKnightCoordiante = tempKnightCoordiante + Point(-1, 1);
                }

                tempKnightCoordiante = blackPieces[i].getCoord();

                knightFoundPiece = false;

                // Knight checks down right diagonal
                while (!isOutOfBounds(tempKnightCoordiante) && !knightFoundPiece)
                {
                    // Find threatened white piece
                    for (int k = 0; k < whitesPieces.size(); k++)
                    {
                        if (whitesPieces[k].getCoord() == (tempKnightCoordiante + Point(1, 1)))
                        {
                            whitesPieces[k].setThreatened(true);

                            knightFoundPiece = true;

                            break;
                        }
                    }

                    // Check if a friendly piece is blocking the patrol path
                    for (int k = 0; k < blackPieces.size(); k++)
                    {
                        if (blackPieces[k].getCoord() == (tempKnightCoordiante + Point(1, 1)))
                        {
                            knightFoundPiece = true;

                            break;
                        }
                    }

                    tempKnightCoordiante = tempKnightCoordiante + Point(1, 1);
                }

                tempKnightCoordiante = blackPieces[i].getCoord();

                knightFoundPiece = false;

                // Knight checks up left diagonal
                while (!isOutOfBounds(tempKnightCoordiante) && !knightFoundPiece)
                {
                    // Find threatened black piece
                    for (int k = 0; k < blackPieces.size(); k++)
                    {
                        if (blackPieces[k].getCoord() == (tempKnightCoordiante + Point(-1, -1)))
                        {
                            blackPieces[k].setThreatened(true);

                            knightFoundPiece = true;

                            break;
                        }
                    }

                    // Check if a friendly piece is blocking the patrol path
                    for (int k = 0; k < blackPieces.size(); k++)
                    {
                        if (blackPieces[k].getCoord() == (tempKnightCoordiante + Point(-1, -1)))
                        {
                            knightFoundPiece = true;

                            break;
                        }
                    }

                    tempKnightCoordiante = tempKnightCoordiante + Point(-1, -1);
                }

                // Knight checks down left diagonal
                while (!isOutOfBounds(tempKnightCoordiante) && !knightFoundPiece)
                {
                    // Find threatened white piece
                    for (int k = 0; k < whitesPieces.size(); k++)
                    {
                        if (whitesPieces[k].getCoord() == (tempKnightCoordiante + Point(1, -1)))
                        {
                            whitesPieces[k].setThreatened(true);

                            knightFoundPiece = true;

                            break;
                        }
                    }

                    // Check if a friendly piece is blocking the patrol path
                    for (int k = 0; k < blackPieces.size(); k++)
                    {
                        if (blackPieces[k].getCoord() == (tempKnightCoordiante + Point(1, -1)))
                        {
                            knightFoundPiece = true;

                            break;
                        }
                    }

                    tempKnightCoordiante = tempKnightCoordiante + Point(1, -1);
                }
            }
        } // Control for black bishops and knights end
    }

public:

    // Constructor
	Board(const string fileName)
	{
        readBoardFile(fileName);

        initPieces();
	}

    // Displays score for each team
    void DisplayPoints()
    {
        // Pieces that are threatened by others are identified
        findThreatened();

        float whiteScore = 0;
        float blackScore = 0;

        // Calculate score of the white team
        for (int i = 0; i < whitesPieces.size(); i++)
        {
            whiteScore += whitesPieces[i].getValue();
        }

        // Calculate score of the black team
        for (int i = 0; i < blackPieces.size(); i++)
        {
            blackScore += blackPieces[i].getValue();
        }

        // Display results
        cout << "White Team Score: " << whiteScore;
        cout << endl;
        cout << "Black Team Score: " << blackScore;
    }

    
		 
};

