#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

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
		NONE,
		PAWN,
		ROOK,
		BISHOP,
		KING,
		QUEEN,
		KNIGHT,
		INVALID,
	};


	Piece(PlayerPiece player_piece, int row, int col, int board_state[][BOARD_LENGTH]);
	Piece(PlayerPiece player_piece, int row, int col);
	Piece(const Piece&);
	~Piece();
	Piece::Types typeOf() const;
	void move(int row, int col, int board_state[][BOARD_LENGTH]);
	std::vector<std::pair<int,int>> get_valid_moves() const;
	std::pair<int,int> get_coords() const;
	void generate_valid_moves(int board_state[][BOARD_LENGTH]);
	int get_team() const;
	bool threatens_king() const;
    bool get_has_moved() const;
    Piece::PlayerPiece get_player_piece()const ;
private:
	std::vector<std::pair<int,int>> valid_moves;

    bool has_moved = false;
	bool is_king_threatened = false;
	int row;
	int col;
	int team; // -1 if White, 1 if Black
	Piece::PlayerPiece player_piece;
	void add_move(int row, int col, int board_state[][BOARD_LENGTH]);
};
