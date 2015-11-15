#include "piece.h"
#define CAN_PLACE_PIECE(a,b) (board_state[a][b] * team <= 0 && board_state[a][b] != Types::INVALID)
#define ADD_MOVE(list, a, b) { if (CAN_PLACE_PIECE(a,b)) list.push_back(std::make_pair(a,b)); \
					is_king_threatened |= (board_state[a][b] * team <= 0 && abs(board_state[a][b]) == Types::KING); }

// All COORDS here are in real coords (1-12)

Piece::Piece(PlayerPiece in_player_piece, int inRow, int inCol, int board_state[][BOARD_LENGTH]) {
	player_piece = in_player_piece;
	row = inRow;
	col = inCol;
	team = player_piece < 0 ? 1 : -1;
	generate_valid_moves(board_state);
}

int Piece::get_team() {
	return team;
}

bool Piece::threatens_king() {
	return is_king_threatened;
}

Piece::Piece(PlayerPiece in_player_piece, int inRow, int inCol) {
	player_piece = in_player_piece;
	row = inRow;
	col = inCol;
	team = player_piece < 0 ? 1 : -1;
	is_king_threatened = false;
}

std::pair<int,int> Piece::get_coords() {
	return std::make_pair(row, col);
}

void Piece::move(int inRow, int inCol, int board_state[][BOARD_LENGTH]) {
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

void Piece::generate_valid_moves(int board_state[][BOARD_LENGTH]) {
	is_king_threatened = false;
	int i;
	Types type = typeOf();
	switch (type) {
		case PAWN:
			// Can only move if no piece
			if (board_state[row+1 * team][col] == Types::NONE)
			ADD_MOVE(valid_moves, row + 1 * team, col);
			if (board_state[row+1 * team][col] == Types::NONE)
			ADD_MOVE(valid_moves, row + 2 * team, col);
			// Can attack diagonally, if there is a piece present
			if (board_state[row+1*team][col+1] * team < 0 && board_state[row+1*team][col+1] != Types::INVALID)
				ADD_MOVE(valid_moves, row + 1 * team, col + 1);
			if (board_state[row+1*team][col-1] * team < 0 && board_state[row+1*team][col-1] != Types::INVALID)
				ADD_MOVE(valid_moves, row + 1 * team, col - 1);
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
		default:
			break;
	}
}

Piece::~Piece() {

}