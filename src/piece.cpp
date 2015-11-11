#include "piece.h"

Piece::Piece(PlayerPiece player_piece, int inRow, int inCol) {
	row = inRow;
	col = inCol;
	team = player_piece > 0 ? 1 : -1;
}

Piece::Types Piece::typeOf() {
	return (Piece::Types)abs(player_piece);
}

std::vector<std::pair<int,int>> Piece::possibleMoves() {
	std::vector<std::pair<int,int>> move_list;
	switch (typeOf()) {
		case PAWN:
			move_list.push_back(std::make_pair(row + 1 * team, col));
			move_list.push_back(std::make_pair(row + 2 * team, col));
			break;
		case ROOK:
			// Horizontal/Vertical
			for (int i = 0; i < BOARD_LENGTH; i++) {
				move_list.push_back(std::make_pair(row, i));
				move_list.push_back(std::make_pair(i, col));
			}
			break;
		case BISHOP:

		case KNIGHT:
			move_list.push_back(std::make_pair(row+2, col + 1));
			move_list.push_back(std::make_pair(row+1, col + 2));
			move_list.push_back(std::make_pair(row-1, col + 2));
			move_list.push_back(std::make_pair(row-2, col+ 1));
			move_list.push_back(std::make_pair(row-2, col - 1));
			move_list.push_back(std::make_pair(row - 1, col - 2));
			move_list.push_back(std::make_pair(row + 1, col - 2));
			move_list.push_back(std::make_pair(row + 2, col - 1));
			break;
		case KING:
			move_list.push_back(std::make_pair(row, col + 1));
			move_list.push_back(std::make_pair(row, col - 1));
			move_list.push_back(std::make_pair(row + 1, col + 1));
			move_list.push_back(std::make_pair(row - 1, col + 1));
			move_list.push_back(std::make_pair(row + 1, col - 1));
			move_list.push_back(std::make_pair(row - 1, col - 1));
			move_list.push_back(std::make_pair(row + 1, col));
			move_list.push_back(std::make_pair(row - 1, col));
			break;
	}
	return move_list;
}

Piece::~Piece() {

}