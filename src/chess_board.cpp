#include "chess_board.h"

ChessBoard::ChessBoard() {
	// First zero out the board
	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			board_state[i][j]=NONE;
		}
	}
	// Place pieces onto the board.
}