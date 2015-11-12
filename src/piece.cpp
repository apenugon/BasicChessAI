#include "piece.h"
#define CAN_PLACE_PIECE(a,b) (board_state[a][b] * team < 0 && board_state[a][b] != Types::INVALID)
#define ADD_MOVE(list, a, b) if (CAN_PLACE_PIECE(a,b)) list.push_back(std::make_pair(a,b));

// All COORDS here are in real coords (1-12)

Piece::Piece(PlayerPiece player_piece, int inRow, int inCol, int** board_state) {
	row = inRow;
	col = inCol;
	team = player_piece < 0 ? 1 : -1;
	generate_valid_moves(board_state);
}

Piece::Piece(PlayerPiece player_piece, int inRow, int inCol) {
	row = inRow;
	col = inCol;
	team = player_piece < 0 ? 1 : -1;
}

std::pair<int,int> Piece::get_coords() {
	return std::make_pair(row, col);
}

void Piece::move(int inRow, int inCol, int** board_state) {
	row = inRow;
	col = inCol;
	generate_valid_moves(board_state);
}

Piece::Types Piece::typeOf() {
	return (Piece::Types)abs(player_piece);
}

std::vector<std::pair<int,int>> Piece::get_valid_moves() {
	return valid_moves;
}

void Piece::generate_valid_moves(int** board_state) {
	int i;
	switch (typeOf()) {
		case PAWN:
			ADD_MOVE(valid_moves, row + 1 * team, col);
			ADD_MOVE(valid_moves, row + 2 * team, col);
			break;
		case QUEEN:
		case ROOK:
			// Horizontal Positive
			i = col;
			do {
				ADD_MOVE(valid_moves, row, i);
				i++;
			} while (board_state[row][i] == NONE);
			// Horizontal Negative
			i = col;
			do {
				ADD_MOVE(valid_moves, i, col);
				i--;
			} while (board_state[row][i] == NONE);
			// Vertical Positive
			i = row;
			do {
				ADD_MOVE(valid_moves, row, i);
				i++;
			} while (board_state[row][i] == NONE);
			// Vertical Negative
			i = row;
			do {
				ADD_MOVE(valid_moves, row, i);
				i--;
			} while (board_state[row][i] == NONE);
			// Allow case for Queen to pass through
			if (typeOf() != QUEEN)
				break;
		case BISHOP:
			// Diagonal Up Right
			i = 0;
			do {
				ADD_MOVE(valid_moves, row+i, col+i);
				i++;
			} while (board_state[row][i] == NONE);
			// Diagonal Down Left
			i = 0;
			do {
				ADD_MOVE(valid_moves, row+i, col+i);
				i--;
			} while (board_state[row][i] == NONE);
			// Diagonal Up Left
			i = 0;
			do {
				ADD_MOVE(valid_moves, row+i, col-i);
				i++;
			} while (board_state[row][i] == NONE);
			// Diagonal Down Right
			i = 0;
			do {
				ADD_MOVE(valid_moves, row+i, col-i);
				i--;
			} while (board_state[row][i] == NONE);
			break;
		case KNIGHT:
			ADD_MOVE(valid_moves, row + 2, col+1);
			ADD_MOVE(valid_moves, row + 1, col+2);
			ADD_MOVE(valid_moves, row - 1, col+2);
			ADD_MOVE(valid_moves, row - 2, col+1);
			ADD_MOVE(valid_moves, row - 2, col-1);
			ADD_MOVE(valid_moves, row - 1, col-2);
			ADD_MOVE(valid_moves, row + 1, col-2);
			ADD_MOVE(valid_moves, row + 2, col-1);
			break;
		case KING:
			ADD_MOVE(valid_moves, row + 1, col+1);
			ADD_MOVE(valid_moves, row, col+1);
			ADD_MOVE(valid_moves, row - 1, col+1);
			ADD_MOVE(valid_moves, row - 1, col);
			ADD_MOVE(valid_moves, row - 1, col-1);
			ADD_MOVE(valid_moves, row, col-1);
			ADD_MOVE(valid_moves, row + 1, col-1);
			ADD_MOVE(valid_moves, row + 1, col);
			break;
	}
}

Piece::~Piece() {

}