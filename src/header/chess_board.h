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
#include <tuple>
#include <functional>

#include "piece.h"

#define NUM_PIECES 7

class ChessBoard {
public:


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

 	enum Teams {
 		WHITE=-1,
 		BLACK=1,
 	};

 	enum MoveType {
 		MOVE,
 		CASTLE,
 	};

 	ChessBoard();
 	ChessBoard(int board_state[][BOARD_LENGTH], 
	std::vector<Piece*> PieceListWhite, 
	std::vector<Piece*> PieceListBlack, 
	Piece* pieceArray[][BOARD_LENGTH]);
 	~ChessBoard();
	int calculateScore();
 	void printBoardState();
 	ChessBoard* makeMove(std::pair<int,int> from, std::pair<int,int> to, Teams team);
 	int getPrevMoveStatusCode();
private:
 	// In our definition for the board, the actual 
 	// board is within the range 2-10. So we have an 8x8
 	// board.

	Teams current_team;

 	char colNumToChar[CHESS_PLAY_LENGTH] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
 	std::map<char, int> colCharToNum;
 	char humanReadablePieces[NUM_PIECES] = {'_', 'P', 'R', 'B', 'K', 'Q', 'N'};

 	int board_state[BOARD_LENGTH][BOARD_LENGTH];
 	std::vector<Piece*> PieceListWhite;
 	std::vector<Piece*> PieceListBlack;
 	Piece* pieceArray[BOARD_LENGTH][BOARD_LENGTH];

 	std::map<char[2], std::vector<char[2]>> possibleMoves;

 	int prevMoveStatusCode;

 	std::vector<std::tuple<MoveType, std::pair<int,int>,std::pair<int,int>>> valid_moves;

 	void initHumanReadablePieces();
 	void initColCharToNum();
 	void newPiece(Piece::PlayerPiece player_piece, int row, int col, Teams team);
 	void removePiece(int row, int col);
 	bool isInCheck();
 	void generate_moves();
 	void placePieces();

};