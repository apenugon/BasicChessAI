#include <vector>
#include <stdlib.h>

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


	Piece(PlayerPiece player_piece, int row, int col, int** board_state);
	Piece(PlayerPiece player_piece, int row, int col);
	~Piece();
	Piece::Types typeOf();
	void move(int row, int col, int** board_state);
	std::vector<std::pair<int,int>> get_valid_moves();
	std::pair<int,int> get_coords();
	void generate_valid_moves(int** board_state);
private:
	std::vector<std::pair<int,int>> valid_moves;

	int row;
	int col;
	int team; // -1 if White, 1 if Black
	Piece::PlayerPiece player_piece;
};