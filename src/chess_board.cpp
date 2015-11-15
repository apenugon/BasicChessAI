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

bool ChessBoard::isInCheck() {
	bool isInCheck = false;
	// Find king
	if (this->current_team == WHITE)
		for (auto p : PieceListWhite) {
			isInCheck |= p->threatens_king();
		}
	else
		for (auto p : PieceListWhite) {
			isInCheck |= p->threatens_king();
		}
	return isInCheck;
	// Check to see if you run into pieces in any direction, 
}

/* Return codes - 
-2 - Can't make that move while King is in check
-1 - to is not valid
0 - There is no piece at from
1 - ok move 
3 - ok, a king is now in check 
4 - ok, player that moved has won
*/
ChessBoard* ChessBoard::makeMove(std::pair<int,int> from, std::pair<int,int> to, Teams team) {

	// Create a vector of pairs, indicating all possible moves - collate those from pieces, then add castling.

	// Step 1: Check to ensure whether or not the king is in Check


	// Step 2: Get the piece that from refers to. Verify that its the correct team moving the piece. If it refers to one, then check the move is in its list.

	// Step 3: Move the piece (involves doing everything on the chess board end as well)  

	// Step 4: Determine whether the King is still in Check. If he is, this is an invalid move

	// ^The above should be done for every possible move. When we enter this state it must be so that the king is not in check

	// Step 5: Whether the game is over.

	return new ChessBoard();
}

void ChessBoard::newPiece(Piece::PlayerPiece player_piece, int row, int col, Teams team) {
	board_state[row][col] = player_piece;
	pieceArray[row][col] = new Piece(player_piece, row, col);
	if (team == WHITE)
		PieceListWhite.push_back(pieceArray[row][col]);
	else
		PieceListBlack.push_back(pieceArray[row][col]);
}

void ChessBoard::removePiece(int row, int col) {
	Teams team = (Teams)pieceArray[row][col]->get_team();
	board_state[row][col] = Piece::Types::NONE;
	auto EraseFun = [row, col](Piece* this_piece) 
				{
					auto coords = this_piece->get_coords(); 
					return std::get<0>(coords) == row 
							&& std::get<1>(coords) == col;
				};
	if (team == WHITE) {
		PieceListWhite.erase(
			std::remove_if(
				PieceListWhite.begin(), 
				PieceListWhite.end(), 
				EraseFun),
			PieceListWhite.end());
	} else {
		PieceListBlack.erase(
			std::remove_if(
				PieceListBlack.begin(), 
				PieceListBlack.end(), 
				EraseFun),
			PieceListBlack.end());
	}
	delete pieceArray[row][col];
}

void ChessBoard::placePieces() {
	// Add pieces to the chess board
	// Note: White starts up top, Black starts on the buttom

	// First add White pieces
	// King is first
	newPiece(Piece::WHITE_KING, BORDER_DEPTH, BORDER_DEPTH + CHESS_PLAY_LENGTH-4, WHITE);

	// White Pawns:
	for (int i = 0; i < CHESS_PLAY_LENGTH; i++) {
		newPiece(Piece::WHITE_PAWN, 1+BORDER_DEPTH, i+BORDER_DEPTH, WHITE);
	}

	//White Rooks
	newPiece(Piece::WHITE_ROOK, BORDER_DEPTH, BORDER_DEPTH, WHITE);
	newPiece(Piece::WHITE_ROOK, BORDER_DEPTH, BORDER_DEPTH + CHESS_PLAY_LENGTH-1, WHITE);
	//White Knights
	newPiece(Piece::WHITE_KNIGHT, BORDER_DEPTH, BORDER_DEPTH+1, WHITE);
	newPiece(Piece::WHITE_KNIGHT, BORDER_DEPTH, BORDER_DEPTH + CHESS_PLAY_LENGTH-2, WHITE);

	//White Bishops
	newPiece(Piece::WHITE_BISHOP, BORDER_DEPTH, BORDER_DEPTH+2, WHITE);
	newPiece(Piece::WHITE_BISHOP, BORDER_DEPTH, BORDER_DEPTH + CHESS_PLAY_LENGTH-3, WHITE);

	//White King
	newPiece(Piece::WHITE_QUEEN, BORDER_DEPTH, BORDER_DEPTH+3, WHITE);
	//White Queen

	//Now add Black pieces

	// King is first, at front of vector
	newPiece(Piece::BLACK_KING, BOARD_LENGTH-BORDER_DEPTH-1, BORDER_DEPTH + CHESS_PLAY_LENGTH-4, BLACK);

	// Black Pawns:
	for (int i = 0; i < CHESS_PLAY_LENGTH; i++) {
		newPiece(Piece::BLACK_PAWN, BOARD_LENGTH-BORDER_DEPTH-2, i+BORDER_DEPTH, BLACK);
	}

	//Black Rooks
	newPiece(Piece::BLACK_ROOK, BOARD_LENGTH-BORDER_DEPTH-1, BORDER_DEPTH, BLACK);
	newPiece(Piece::BLACK_ROOK, BOARD_LENGTH-BORDER_DEPTH-1, BORDER_DEPTH + CHESS_PLAY_LENGTH-1, BLACK);
	//Black Knights
	newPiece(Piece::BLACK_KNIGHT, BOARD_LENGTH-BORDER_DEPTH-1, BORDER_DEPTH+1, BLACK);
	newPiece(Piece::BLACK_KNIGHT, BOARD_LENGTH-BORDER_DEPTH-1, BORDER_DEPTH + CHESS_PLAY_LENGTH-2, BLACK);

	//Black Bishops
	newPiece(Piece::BLACK_BISHOP, BOARD_LENGTH-BORDER_DEPTH-1, BORDER_DEPTH+2, BLACK);
	newPiece(Piece::BLACK_BISHOP, BOARD_LENGTH-BORDER_DEPTH-1, BORDER_DEPTH + CHESS_PLAY_LENGTH-3, BLACK);

	//Black King
	newPiece(Piece::BLACK_QUEEN, BOARD_LENGTH-BORDER_DEPTH-1, BORDER_DEPTH+3, BLACK);
	//Black Queen


}

void ChessBoard::generate_moves() {
	// Only generate moves for the current team
	std::vector<Piece*>* useVector = this->current_team == WHITE ? &PieceListWhite : &PieceListBlack;

	// Note - if in check, only the king counts. Otherwise, everything does
	if (isInCheck()) {
		auto p = useVector->front();
		for (auto move : p->get_valid_moves()) {
			auto move_tuple = std::make_tuple(MOVE, p->get_coords(), move);
			valid_moves.push_back(move_tuple);
		}
	} else {
		// Generate move sets for each piece, add them to the actual valid move list
		for (std::vector<Piece*>::iterator p = useVector->begin(); p < useVector->end(); ++p) {
			(*p)->generate_valid_moves(board_state);
			for (auto move : (*p)->get_valid_moves()) {
				auto move_tuple = std::make_tuple(MOVE, (*p)->get_coords(), move);
				valid_moves.push_back(move_tuple);
			}
		}
	}

	// Add in Castling moves
	// 6 different conditions

	// Now filter all moves based on whether or not the king is in check afterwards (probably the most computationally intensive)
}

// Creates a new chessboard, set up with the next board state.
ChessBoard::ChessBoard(int board_state[][BOARD_LENGTH], 
	std::vector<Piece*> PieceListWhite, 
	std::vector<Piece*> PieceListBlack, 
	Piece* pieceArray[][BOARD_LENGTH]) {
	initColCharToNum();

	this->PieceListWhite = PieceListWhite;
	this->PieceListBlack = PieceListBlack;
	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			this->board_state[i][j] = board_state[i][j];
			this->pieceArray[i][j] = pieceArray[i][j];
		}
	}

}

// Creates a _new_ chessboard, set up with a standard chess start
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
			pieceArray[i][j] = NULL;
		}
	}
	// Place pieces on chess board
	placePieces();
	generate_moves();
	this->current_team = WHITE;
}

ChessBoard::~ChessBoard() {
	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			if (pieceArray[i][j] != NULL)
				delete pieceArray[i][j];
		}
	}
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
	cout << "   a  b  c  d  e  f  g  h " << endl << endl;
}