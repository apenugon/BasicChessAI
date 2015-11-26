#ifndef CHESSBOARD_H
#define CHESSBOARD_H

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
#include <string>

#include "move.h"
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

    ChessBoard();
 	ChessBoard(int board_state[][BOARD_LENGTH], 
		std::vector<Piece*> PieceListWhite, 
		std::vector<Piece*> PieceListBlack, 
		Piece* pieceArray[][BOARD_LENGTH],
		Teams team,
		bool do_generate_moves = true);
 	~ChessBoard();
	int calculateScore();
 	void printBoardState();
 	ChessBoard* makeMove(Move move, bool is_valid = false, bool do_generate_moves = true);
 	void print_available_moves();
 	void check_integrity();
 	bool is_game_over();
    int num_available_moves();
    std::vector<Move> get_valid_moves();
    ChessBoard::Teams get_team();
    ChessBoard::Teams get_opposing_team();
    std::vector<Piece*> getPieceList(ChessBoard::Teams team);
    bool square_in_danger(int row, int col);
    bool isInCheck(Teams team);
private:
 	// In our definition for the board, the actual 
 	// board is within the range 2-10. So we have an 8x8
 	// board.

    // Vars that contain info from past game boards
	Teams current_team;

 	char humanReadablePieces[NUM_PIECES] = {'_', 'P', 'R', 'B', 'K', 'Q', 'N'};
    char colNumToChar[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

 	int board_state[BOARD_LENGTH][BOARD_LENGTH];
 	std::vector<Piece*> PieceListWhite;
 	std::vector<Piece*> PieceListBlack;
 	Piece* pieceArray[BOARD_LENGTH][BOARD_LENGTH];

 	std::map<char[2], std::vector<char[2]>> possibleMoves;

 	int prevMoveStatusCode;

 	std::vector<Move> valid_moves;

 	void newPiece(Piece::PlayerPiece player_piece, int row, int col, Teams team);	
 	void generate_piece_moves();
 	void generate_moves();
 	void placePieces();
 	void move_piece(Piece *piece, 
				int toRow, 
				int toCol, 
				int new_board_state[][BOARD_LENGTH], 
				Piece* new_pieceArray[][BOARD_LENGTH]);
};
#endif
