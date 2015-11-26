#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <stdexcept>

#define CHESS_PLAY_LENGTH 8
#define BORDER_DEPTH 2
#define BOARD_LENGTH CHESS_PLAY_LENGTH + 2 * BORDER_DEPTH

class Piece {
public:
	enum PlayerPiece {
		BLACK_KNIGHT=6,
		BLACK_QUEEN=5,
		BLACK_KING=4,
		BLACK_BISHOP=3,
		BLACK_ROOK=2,
		BLACK_PAWN=1,
		WHITE_PAWN=-1,
		WHITE_ROOK=-2,
		WHITE_BISHOP=-3,
		WHITE_KING=-4,
		WHITE_QUEEN=-5,
		WHITE_KNIGHT=-6,
	};

 	enum Types {
		NONE=0,
		PAWN=1,
		ROOK=2,
		BISHOP=3,
		KING=4,
		QUEEN=5,
		KNIGHT=6,
		INVALID=7,
	};


	Piece(PlayerPiece player_piece, int row, int col, int board_state[][BOARD_LENGTH]);
	Piece(PlayerPiece player_piece, int row, int col);
	~Piece();
	Piece::Types typeOf() const;
	void move(int row, int col, int board_state[][BOARD_LENGTH]);
	std::vector<std::pair<int,int>> get_valid_moves() const;
	std::pair<int,int> get_coords() const;
	void generate_valid_moves(int board_state[][BOARD_LENGTH]);
	int get_team() const;
	bool threatens_king() const;
    bool get_has_moved() const;
    void promote(Piece::Types type);
    Piece::PlayerPiece get_player_piece()const ;
    void check_integrity();
    bool is_special_move_used();
    int get_same_pos_counter();
    int get_move_counter();
    bool get_can_promote();
private:
	std::vector<std::pair<int,int>> valid_moves;

    bool can_promote = false;
    bool has_moved = false;
	bool is_king_threatened = false;
	int row;
	int col;
    int team; // -1 if White, 1 if Black
    bool special_move_used = false;
    int same_pos_counter = 0;
    int move_counter = 0;
	Piece::PlayerPiece player_piece;
	void add_move(int row, int col, int board_state[][BOARD_LENGTH]);
};

#endif
