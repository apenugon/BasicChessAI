/*----------------------------------------------------------
 *
 *	Class ChessBoard - stores the state of the chess board
 *
 *----------------------------------------------------------
 */
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#include "piece.h"

#define NUM_PIECES 7

class ChessBoard {
public:

 	ChessBoard();
 	~ChessBoard();
	int calculateScore();
 	void placePieces();
 	void printBoardState();

 	enum Col {
 		a=2,
 		b=3, 
 		c=4, 
 		d=5, 
 		e=6, 
 		f=7,
 		g=8, 
 		h=9,
 	};

private:
 	// In our definition for the board, the actual 
 	// board is within the range 2-10. So we have an 8x8
 	// board.
 	char colNumToChar[CHESS_PLAY_LENGTH] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
 	std::map<char, int> colCharToNum;
 	char humanReadablePieces[NUM_PIECES] = {'_', 'P', 'R', 'B', 'K', 'Q', 'N'};

 	int board_state[BOARD_LENGTH][BOARD_LENGTH];
 	std::vector<Piece*> PieceList;
 	std::map<char[2], std::vector<char[2]>> possibleMoves;

 	Piece* pieceArray[BOARD_LENGTH][BOARD_LENGTH];

 	void initHumanReadablePieces();
 	void initColCharToNum();
 	void newPiece(Piece::PlayerPiece player_piece, int row, int col);
 	void removePiece(int row, int col);
};