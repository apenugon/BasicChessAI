/*----------------------------------------------------------
 *
 *	Class ChessBoard - stores the state of the chess board
 *
 *----------------------------------------------------------
 */
#include <vector>

#define BOARD_LENGTH 12

class ChessBoard {
public:
	int calculateScore();
 	void placePiece();
 	void printBoardState();

 	enum Pieces {
		EMPTY,
		PAWN,
		ROOK,
		BISHOP,
		KING,
		QUEEN,
		KNIGHT,
	};

	enum PlayerPieces {
		BLACK_KNIGHT=6,
		BLACK_QUEEN=5,
		BLACK_KING=4,
		BLACK_BISHOP=3,
		BLACK_ROOK=2,
		BLACK_PAWN=1,
		NONE=0,
		WHITE_PAWN=-1,
		WHITE_ROOK=-2,
		WHITE_BISHOP=-3,
		WHITE_KING=-4,
		WHITE_QUEEN=-5,
		WHITE_KNIGHT=-6,
	};

private:
 	ChessBoard();
 	~ChessBoard();
 	// In our definition for the board, the actual 
 	// board is within the range 2-10. So we have an 8x8
 	// board.
 	int board_state[12][12];
 	std::vector<int> PieceList;
};