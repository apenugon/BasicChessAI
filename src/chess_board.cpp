#include "chess_board.h"

using namespace std;

void ChessBoard::initColCharToNum() {
	colCharToNum['a'] = 0;
	colCharToNum['b'] = 1;
	colCharToNum['c'] = 2;
	colCharToNum['d'] = 3;
	colCharToNum['e'] = 4;
	colCharToNum['f'] = 5;
	colCharToNum['g'] = 6;
	colCharToNum['h'] = 7;
}

void ChessBoard::placePieces() {
	// Add pieces to the chess board
	// Note: White starts up top, Black starts on the buttom

	// First add White pieces
	// White Pawns:
	for (int i = 0; i < CHESS_PLAY_LENGTH; i++) {
		board_state[b][i+BORDER_DEPTH] = Piece::WHITE_PAWN;
	}

	//White Rooks
	board_state[a][BORDER_DEPTH] = Piece::WHITE_ROOK;
	board_state[a][BORDER_DEPTH+CHESS_PLAY_LENGTH-1] = Piece::WHITE_ROOK;

	//White Knights
	board_state[a][BORDER_DEPTH+1] = Piece::WHITE_KNIGHT;
	board_state[a][BORDER_DEPTH+CHESS_PLAY_LENGTH-2] = Piece::WHITE_KNIGHT;

	//White Bishops
	board_state[a][BORDER_DEPTH+2] = Piece::WHITE_BISHOP;
	board_state[a][BORDER_DEPTH+CHESS_PLAY_LENGTH-3] = Piece::WHITE_BISHOP;

	//White King
	board_state[a][BORDER_DEPTH+3] = Piece::WHITE_QUEEN;
	//White Queen
	board_state[a][BORDER_DEPTH+CHESS_PLAY_LENGTH-4] = Piece::WHITE_KING;

	//Now add Black pieces
	// Black Pawns:
	for (int i = 0; i < CHESS_PLAY_LENGTH; i++) {
		board_state[g][i+BORDER_DEPTH] = Piece::BLACK_PAWN;
	}

	//Black Rooks
	board_state[h][BORDER_DEPTH] = Piece::BLACK_ROOK;
	board_state[h][BORDER_DEPTH+CHESS_PLAY_LENGTH-1] = Piece::BLACK_ROOK;

	//Black Knights
	board_state[h][BORDER_DEPTH+1] = Piece::BLACK_KNIGHT;
	board_state[h][BORDER_DEPTH+CHESS_PLAY_LENGTH-2] = Piece::BLACK_KNIGHT;

	//Black Bishops
	board_state[h][BORDER_DEPTH+2] = Piece::BLACK_BISHOP;
	board_state[h][BORDER_DEPTH+CHESS_PLAY_LENGTH-3] = Piece::BLACK_BISHOP;

	//Black King
	board_state[h][BORDER_DEPTH+3] = Piece::BLACK_KING;
	//Black Queen
	board_state[h][BORDER_DEPTH+CHESS_PLAY_LENGTH-4] = Piece::BLACK_QUEEN;
}

ChessBoard::ChessBoard() {
	// Initialize the map colCharToNum
	initColCharToNum();
	// First zero out the board (Or set to be invalid)
	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			// Mark regions that are out of bounds as such
			if (i < BORDER_DEPTH || 
				j < BORDER_DEPTH || 
				i >= BOARD_LENGTH - BORDER_DEPTH || 
				j >= BOARD_LENGTH - BORDER_DEPTH)
				board_state[i][j]=Piece::Types::INVALID;
			else
				board_state[i][j]=Piece::Types::NONE;
		}
	}

	placePieces();
	
}

void ChessBoard::printBoardState() {
	cout << "  ----------------------- " << endl;
	for (int i = BOARD_LENGTH-BORDER_DEPTH-1; i >= BORDER_DEPTH; i--) {
		cout << i-1 << "|";
		for (int j = BORDER_DEPTH; j < BOARD_LENGTH-BORDER_DEPTH; j++) {
			char neg = board_state[i][j] < 0 ? '-' : ' ';
			cout << neg << humanReadablePieces[abs(board_state[i][j])] << "|";
		}
		cout << endl;
	}
	cout << "  ----------------------- " << endl;
	cout << "   a| b| c| d| e| f| g| h" << endl;

#ifdef DEBUG
	// Print contents of the pieces here
	cout << "Test" << endl;
#endif
}