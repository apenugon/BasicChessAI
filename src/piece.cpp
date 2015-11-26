#include "piece.h"
#define CAN_PLACE_PIECE(a,b) (board_state[a][b] * team <= 0 && board_state[a][b] != Types::INVALID)
#define TEST_OPPOSING_PIECE(a,b) if (board_state[a][b] * team < 0) break;

// All COORDS here are in real coords (1-12)

void Piece::add_move(int row, int col, int board_state[][BOARD_LENGTH]) {
	if (CAN_PLACE_PIECE(row,col)) {
#ifdef DEBUG
    int orow;
    int ocol;
    std::tie(orow, ocol) = get_coords();
    if (row == orow && col == ocol) {
        std::cout << "Moving to same position!" << std::endl;
    }
#endif
		auto coord = std::make_pair(row,col);
		valid_moves.push_back(coord);
		is_king_threatened |= (abs(board_state[row][col]) == Types::KING);
    } 
}

bool Piece::get_can_promote() {
    return can_promote;
}

Piece::PlayerPiece Piece::get_player_piece() const {
	return player_piece;
}

Piece::Piece(PlayerPiece in_player_piece, int inRow, int inCol, int board_state[][BOARD_LENGTH]) {
	player_piece = in_player_piece;
	row = inRow;
	col = inCol;
	team = player_piece < 0 ? -1 : 1;
}

int Piece::get_team() const {
	return team;
}

bool Piece::threatens_king() const {
	return is_king_threatened;
}

Piece::Piece(PlayerPiece in_player_piece, int inRow, int inCol) {
	player_piece = in_player_piece;
	row = inRow;
	col = inCol;
	team = player_piece < 0 ? -1 : 1;
	is_king_threatened = false;
}

std::pair<int,int> Piece::get_coords() const {
	return std::make_pair(row, col);
}

bool Piece::get_has_moved() const {
    return this->has_moved;
}

int Piece::get_move_counter() {
    return this->move_counter;
}

void Piece::move(int inRow, int inCol, int board_state[][BOARD_LENGTH]) {
	// Set whether or not pawn moved forward twice - used for En Pessent
    if (this->typeOf() == PAWN) {
        this->special_move_used |= abs(inRow - this->row) == 2;
    }

    this->move_counter++;
    this->same_pos_counter = 0;

    row = inRow;
	col = inCol;
    this->has_moved = true;
    this->is_king_threatened = false;

    // Metamorphosize Pawn into Queen if its at the last row
    // Actually this should be done at the board level
}

void Piece::promote(Piece::Types type) {
    this->player_piece = (Piece::PlayerPiece)(type * this->team);
}

bool Piece::is_special_move_used() {
    return special_move_used;
}

int Piece::get_same_pos_counter() {
    return same_pos_counter;
}

Piece::Types Piece::typeOf() const {
	return (Piece::Types)abs(player_piece);
}

std::vector<std::pair<int,int>> Piece::get_valid_moves() const {
	return valid_moves;
}

void Piece::generate_valid_moves(int board_state[][BOARD_LENGTH]) {
	#ifdef DEBUG
	//std::cout << player_piece << " generating moves " << "Team: " << team << std::endl;
	//std::cout << "Coords: " << this->row << " : " << this->col << "has moved " << has_moved << std::endl;
	#endif
	valid_moves.clear(); // Starting from scratch
    same_pos_counter++;

    this->can_promote = false;
	this->is_king_threatened = false;
	int i;
	int move_factor = team * -1;
	Types type = typeOf();
	switch (type) {
		case PAWN: // Inverse of team number
			// Can only move if no piece
			if (board_state[row+1 * move_factor][col] == Types::NONE)
			add_move(row + 1 * move_factor, col, board_state);
			if (board_state[row+1 * move_factor][col] == Types::NONE && (row == 3 || row == 8))
			add_move(row + 2 * move_factor, col, board_state);
			// Can attack diagonally, if there is a piece present
			if (board_state[row+1*move_factor][col+1] * team < 0 && board_state[row+1*move_factor][col+1] != Types::INVALID)
				add_move(row + 1 * move_factor, col + 1, board_state);
			if (board_state[row+1*move_factor][col-1] * team < 0 && board_state[row+1*move_factor][col-1] != Types::INVALID)
				add_move(row + 1 * move_factor, col - 1, board_state);
            this->can_promote = ((team == -1 && row == 8) ||
                (team == 1 && row == 3));
			break;
		case QUEEN:
		case ROOK:
			// Horizontal Positive
			i = col;
			do {
				add_move(row, i, board_state);
                TEST_OPPOSING_PIECE(row, i);
				i++;
			} while (CAN_PLACE_PIECE(row, i));
			// Horizontal Negative
			i = col;
			do {
				add_move(row, i, board_state);
                TEST_OPPOSING_PIECE(row, i);
				i--;
			} while (CAN_PLACE_PIECE(row, i));
			// Vertical Positive
			i = row;
			do {
				add_move(i, col, board_state);
                TEST_OPPOSING_PIECE(i, col);
				i++;
			} while (CAN_PLACE_PIECE(i, col));
			// Vertical Negative
			i = row;
			do {
				add_move(i, col, board_state);
                TEST_OPPOSING_PIECE(i, col);
				i--;
			} while (CAN_PLACE_PIECE(i, col));
			// Allow case for Queen to pass through
			if (typeOf() != QUEEN)
				break;
		case BISHOP:
			// Diagonal Up Right
			i = 0;
			do {
				add_move(row+i, col+i, board_state);
                TEST_OPPOSING_PIECE(row+i, col+i);
				i++;
			} while (CAN_PLACE_PIECE(row+i, col+i));
			// Diagonal Down Left
			i = 0;
			do {
				add_move(row+i, col+i, board_state);
                TEST_OPPOSING_PIECE(row+i, col+i);
				i--;
			} while (CAN_PLACE_PIECE(row+i, col+i));
			// Diagonal Up Left
			i = 0;
			do {
				add_move(row+i, col-i, board_state);
                TEST_OPPOSING_PIECE(row+i, col-i);
				i++;
			} while (CAN_PLACE_PIECE(row+i, col-i));
			// Diagonal Down Right
			i = 0;
			do {
				add_move(row+i, col-i, board_state);
                TEST_OPPOSING_PIECE(row+i, col-i);
				i--;
			} while (CAN_PLACE_PIECE(row+i,col-i));
			break;
		case KNIGHT:
			add_move(row + 2, col+1, board_state);
			add_move(row + 1, col+2, board_state);
			add_move(row - 1, col+2, board_state);
			add_move(row - 2, col+1, board_state);
			add_move(row - 2, col-1, board_state);
			add_move(row - 1, col-2, board_state);
			add_move(row + 1, col-2, board_state);
			add_move(row + 2, col-1, board_state);
			break;
		case KING:
			add_move(row + 1, col+1, board_state);
			add_move(row, col+1, board_state);
			add_move(row - 1, col+1, board_state);
			add_move(row - 1, col, board_state);
			add_move(row - 1, col-1, board_state);
			add_move(row, col-1, board_state);
			add_move(row + 1, col-1, board_state);
			add_move(row + 1, col, board_state);
			break;
		default:
			break;
	}
}

Piece::~Piece() {

}

void Piece::check_integrity() {
	// Checks integrity of piece
	bool row_in_bounds = 0 <= this->row && this->row < BOARD_LENGTH;
	bool col_in_bounds = 0 <= this->col && this->col < BOARD_LENGTH;
	if (!row_in_bounds) {
		throw std::runtime_error("Row " + std::to_string(this->row) + " is not in bounds for piece " + std::to_string(player_piece));
	} 

	if (!col_in_bounds) {
		throw std::runtime_error("Col " + std::to_string(this->col) + " is not in bounds for piece " + std::to_string(player_piece));
	}

	if (this->team * this->player_piece <= 0) {
		throw std::runtime_error("Team mismatch: Piece " + std::to_string(this->player_piece) + " Team " + std::to_string(this->team));
	}

}
