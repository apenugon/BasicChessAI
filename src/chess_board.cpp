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



void ChessBoard::print_available_moves() {
	std::string team = current_team == WHITE ? "WHITE" : "BLACK";
	std::cout << "Available Moves for " << team << std::endl;
	for (auto m : valid_moves) {
		std::string move = std::get<0>(m) == MOVE ? "MOVE" : "CASTLE";
		auto from = std::get<1>(m);
		auto to = std::get<2>(m);
		int from_row = std::get<0>(from);
		int from_col = std::get<1>(from);
		int to_row = std::get<0>(to);
		int to_col = std::get<1>(to);
		std::cout << move << " " << from_row-BORDER_DEPTH+1 << colNumToChar[from_col-BORDER_DEPTH] << " " << to_row-BORDER_DEPTH+1 << colNumToChar[to_col-BORDER_DEPTH] << std::endl;
	}
#ifdef DEBUG
    std::cout << "Pieces Present:" << std::endl;
    // Print out every piece present
    for (auto p : PieceListWhite) {
        std::cout << p->get_player_piece() << std::endl;
    }
    for (auto p : PieceListBlack) {
        std::cout << p->get_player_piece() << std::endl;
    }
#endif
}

bool ChessBoard::isInCheck(Teams team) {
	bool isInCheck = false;
	// Find king
	if (team == WHITE)
		for (auto p : PieceListBlack) {
			isInCheck |= p->threatens_king();
		}
	else
		for (auto p : PieceListWhite) {
			isInCheck |= p->threatens_king();
		}
	return isInCheck;
	// Check to see if you run into pieces in any direction, 
}

void ChessBoard::move_piece(Piece *piece, 
				int toRow, 
				int toCol, 
				int new_board_state[][BOARD_LENGTH], 
				Piece* new_pieceArray[][BOARD_LENGTH]) {
	auto original_position = piece->get_coords();
	new_board_state[std::get<0>(original_position)][std::get<1>(original_position)] = Piece::Types::NONE;
	new_board_state[toRow][toCol] = board_state[std::get<0>(original_position)][std::get<1>(original_position)];
	new_pieceArray[std::get<0>(original_position)][std::get<1>(original_position)] = NULL;
	new_pieceArray[toRow][toCol] = piece;
	piece->move(toRow, toCol, new_board_state);
}

ChessBoard* ChessBoard::makeMove(std::tuple<MoveType, std::string, std::string> move) {
	return makeMove(std::get<0>(move), std::get<1>(move), std::get<2>(move));
}

ChessBoard* ChessBoard::makeMove(std::tuple<MoveType, std::pair<int,int>, std::pair<int,int>> move) {
	return makeMove(std::get<0>(move), std::get<1>(move), std::get<2>(move));
}

ChessBoard* ChessBoard::makeMove(MoveType move_type, std::string from, std::string to) {
	std::pair<int,int> from_pair = std::make_pair(from.at(0) - '0' + BORDER_DEPTH - 1, colCharToNum[from.at(1)]+BORDER_DEPTH);
	std::pair<int,int> to_pair = std::make_pair(to.at(0) - '0' + BORDER_DEPTH - 1, colCharToNum[to.at(1)]+BORDER_DEPTH);
#ifdef DEBUG
    std::cout << "Making given move: (" << std::get<0>(from_pair) << "," << std::get<1>(from_pair) << "),(";
    std::cout << std::get<0>(to_pair) << "," << std::get<1>(to_pair) << ")" << std::endl;
#endif
	return makeMove(move_type, from_pair, to_pair);
}

ChessBoard* ChessBoard::makeMove(MoveType move_type, std::pair<int,int> from, std::pair<int,int> to, bool is_valid, bool do_generate_moves) {
	auto in_bounds = [] (std::pair<int,int> in_coords) {return 0 <= std::get<0>(in_coords) && std::get<0>(in_coords) < BOARD_LENGTH &&
                                                                0 <= std::get<1>(in_coords) && std::get<1>(in_coords) < BOARD_LENGTH;};
    auto move = std::make_tuple(move_type, from, to);

	// Validate move
	if (!is_valid) {
		if (!in_bounds(from) || !in_bounds(to))
        	return NULL;
		for (auto m : valid_moves) {
			if (m == move)
				is_valid = true;
		}

		if (!is_valid)
			return NULL;
	}

	// Construct a new board

	auto next_team = current_team == WHITE ? BLACK : WHITE;
	int new_board_state[BOARD_LENGTH][BOARD_LENGTH];
	Piece* new_pieceArray[BOARD_LENGTH][BOARD_LENGTH];
	std::vector<Piece*> newPieceListWhite;
	std::vector<Piece*> newPieceListBlack;

	// Here I need to copy over both piece lists into a new piece list. The pieces need to be distinct

    int fromRow = std::get<0>(from);
    int fromCol = std::get<1>(from);
    int toRow = std::get<0>(to);
    int toCol = std::get<1>(to);

    // Check for En Pessent
    if (abs(fromRow - toRow) == 1 &&
        abs(fromCol - toCol) == 1 &&
        pieceArray[fromRow][fromCol]->typeOf() == Piece::Types::PAWN &&
        pieceArray[toRow][toCol] == NULL) {
        toRow = fromRow;
    }

    for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			new_board_state[i][j] = board_state[i][j];
			if (pieceArray[i][j] != NULL) {
				if (move_type == MOVE && i == toRow && j == toCol) {
					// This piece is being taken - we don't need it in the next board
					new_pieceArray[i][j] = NULL;
					new_board_state[i][j] = Piece::Types::NONE;
				} else {
					// We need to create a copy of this piece for the next board
					Piece* newPiece = new Piece(*(pieceArray[i][j]));
					new_pieceArray[i][j] = newPiece;
					if (newPiece->get_team() == WHITE)
						newPieceListWhite.push_back(newPiece);
					else
						newPieceListBlack.push_back(newPiece);
					}
			} else {
				new_pieceArray[i][j] = NULL;
			}

		}
	} 

	switch (move_type) {
		case CASTLE: {
			auto king = new_pieceArray[std::get<0>(from)][std::get<1>(from)];
			auto rook = new_pieceArray[std::get<0>(to)][std::get<1>(to)];
			// Determine if this is a left castle or a right castle
			if (std::get<1>(to) < std::get<1>(from)) {
				// Left Castle
				move_piece(king, std::get<0>(from), std::get<1>(from)-2, new_board_state, new_pieceArray);
				move_piece(rook, std::get<0>(to), std::get<1>(to)+3, new_board_state, new_pieceArray);
			} else {
				// Right Castle
				move_piece(king, std::get<0>(from), std::get<1>(from)+2, new_board_state, new_pieceArray);
				move_piece(rook, std::get<0>(to), std::get<1>(to)-2, new_board_state, new_pieceArray);
			}
			break;
		}
		default:
			// Just do a normal move
			auto piece_to_move = new_pieceArray[std::get<0>(from)][std::get<1>(from)];
			move_piece(piece_to_move, std::get<0>(to), std::get<1>(to), new_board_state, new_pieceArray);
	}


	return new ChessBoard(new_board_state, newPieceListWhite, newPieceListBlack, new_pieceArray, next_team, do_generate_moves);
}

void ChessBoard::newPiece(Piece::PlayerPiece player_piece, int row, int col, Teams team) {
	board_state[row][col] = player_piece;
	pieceArray[row][col] = new Piece(player_piece, row, col);
	if (team == WHITE)
		PieceListWhite.push_back(pieceArray[row][col]);
	else
		PieceListBlack.push_back(pieceArray[row][col]);
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

void ChessBoard::generate_piece_moves() {
    // First check if there are any pieces to be promoted, then
	// Generate opposing pieces' moves.
	for (auto p : PieceListBlack) {
        if (p->promote()) {
            board_state[std::get<0>(p->get_coords())][std::get<1>(p->get_coords())] = (int)(p->get_player_piece());
        }
		p->generate_valid_moves(board_state);
	}
	for (auto p : PieceListWhite) {
        if (p->promote()) {
            board_state[std::get<0>(p->get_coords())][std::get<1>(p->get_coords())] = (int)(p->get_player_piece());
        }
		p->generate_valid_moves(board_state);
	}
}

bool ChessBoard::square_in_danger(int row, int col) {
	std::vector<Piece*>* opposing_pieces = this->current_team == WHITE ? &PieceListBlack : &PieceListWhite;
	for (auto p : (*opposing_pieces)) {
		for (auto m : p->get_valid_moves()) {
			if (row == std::get<0>(m) && col == std::get<1>(m)) {
				return true;
			}
		}
	}
	return false;
}

void ChessBoard::generate_moves() {
	// Only generate moves for the current team
	std::vector<Piece*>* useVector = this->current_team == WHITE ? &PieceListWhite : &PieceListBlack;
	int castle_row = this->current_team == WHITE ? BORDER_DEPTH : BOARD_LENGTH - BORDER_DEPTH - 1;

	Piece* king;
	for (auto p : (*useVector)) {
		if (p->typeOf() == Piece::Types::KING) {
			king = p;
			break;
		}
	}


	// Generate move sets for each piece, add them to the actual valid move list
	for (std::vector<Piece*>::iterator p = useVector->begin(); p < useVector->end(); ++p) {
		for (auto move : (*p)->get_valid_moves()) {
			auto move_tuple = std::make_tuple(MOVE, (*p)->get_coords(), move);
			valid_moves.push_back(move_tuple);
		}
	}

    // Generate En Passent moves
    for (auto p : (*useVector)) {
        if (p->typeOf() == Piece::Types::PAWN) {
            // Now check for En Passent conditions
            int row = std::get<0>(p->get_coords());
            int col = std::get<1>(p->get_coords());

            // Left Side:
            if (pieceArray[row][col-1] != NULL &&
                pieceArray[row][col-1]->is_special_move_used() == true &&
                pieceArray[row][col-1]->get_same_pos_counter() == 1 &&
                pieceArray[row][col-1]->get_move_counter() == 1 &&
                pieceArray[row][col-1]->get_team() != this->current_team) {
                auto move_tuple = std::make_tuple(MOVE, p->get_coords(), std::make_pair(row-current_team, col-1));
                valid_moves.push_back(move_tuple);
            }
            // Right
            if (pieceArray[row][col+1] != NULL &&
                pieceArray[row][col+1]->is_special_move_used() == true &&
                pieceArray[row][col+1]->get_same_pos_counter() == 1 &&
                pieceArray[row][col+1]->get_move_counter() == 1 &&
                pieceArray[row][col+1]->get_team() != this->current_team) {
                auto move_tuple = std::make_tuple(MOVE, p->get_coords(), std::make_pair(row-current_team, col+1));
                valid_moves.push_back(move_tuple);
            }
        }
}

	// Add in Castling moves
	// 6 different conditions

    auto coords = king->get_coords();
    int kingCol = std::get<1>(coords);

	// Check if king was moved and King not in check
	if (!isInCheck(this->current_team) && king->get_has_moved() == false) {
		// Rook Left
		auto rook = pieceArray[castle_row][a];
		if (rook != NULL && 
			rook ->get_has_moved() == false &&
			rook->typeOf() == Piece::Types::ROOK) {
			// Check if the intervening spaces are free
			bool areFree = true;
			for (int i = kingCol - 1; i > a; i--) {
				// Also check if we are castling through check
				if (board_state[castle_row][i] != Piece::Types::NONE ||
					square_in_danger(castle_row, i)) {
					areFree = false;
					break;
				}
			}
			if (areFree) {
				auto move = std::make_tuple(CASTLE, coords, rook->get_coords());
				valid_moves.push_back(move);
			}
		}
		// Rook Right
		rook = pieceArray[castle_row][h];
		if (rook != NULL && 
			rook ->get_has_moved() == false &&
			rook->typeOf() == Piece::Types::ROOK) {
			// Check if the intervening spaces are free
			bool areFree = true;
			for (int i = kingCol + 1; i < h; i++) {
				if (board_state[castle_row][i] != Piece::Types::NONE ||
					square_in_danger(castle_row, i)) {
					areFree = false;
					break;
				}
			}
			if (areFree) {
				auto move = std::make_tuple(CASTLE, king->get_coords(), rook->get_coords());
				valid_moves.push_back(move);
			}
		}

	}

	//Are there pieces in between?
	auto filter_function = [this] (std::tuple<MoveType, std::pair<int,int>,std::pair<int,int>> move) {
		// Make move, determine if king in check
		#ifdef DEBUG
		int a = std::get<0>(std::get<1>(move));
		int b = std::get<1>(std::get<1>(move));
		int c = std::get<0>(std::get<2>(move));
		int d = std::get<1>(std::get<2>(move));
		std::cout << "Filtering move " << a << "," << b << " " << c << "," <<  d << std::endl;
		#endif
		ChessBoard* nextBoard = this->makeMove(std::get<0>(move), std::get<1>(move), std::get<2>(move), true, false);
		bool is_invalid_move = nextBoard->isInCheck(current_team);
		delete nextBoard;
		return is_invalid_move;
	};

    // Filters the moves by whether or not the king is present
	valid_moves.erase(std::remove_if(valid_moves.begin(), valid_moves.end(), filter_function), valid_moves.end());
}

// Creates a new chessboard, set up with the next board state.
ChessBoard::ChessBoard(int board_state[][BOARD_LENGTH], 
	std::vector<Piece*> PieceListWhite, 
	std::vector<Piece*> PieceListBlack, 
	Piece* pieceArray[][BOARD_LENGTH],
	Teams team,
	bool do_generate_moves) {
	initColCharToNum();

	this->PieceListWhite = PieceListWhite;
	this->PieceListBlack = PieceListBlack;
	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			this->board_state[i][j] = board_state[i][j];
			this->pieceArray[i][j] = pieceArray[i][j];
		}
	}

	this->current_team = team;

	generate_piece_moves();
	if (do_generate_moves) {
		#ifdef DEBUG
		std::cout << "Generate Moves" << std::endl;
		#endif
		generate_moves();
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
	this->current_team = WHITE;
	// Place pieces on chess board
	placePieces();
	generate_piece_moves();
	generate_moves();
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
			char neg = board_state[i][j] < 0 ? 'W' : board_state[i][j] > 0 ? 'B' : ' ';
			cout << neg << humanReadablePieces[abs(board_state[i][j])] << "|";
		}
		cout << endl;
	}
	cout << "  ----------------------- " << endl;
	cout << "   a  b  c  d  e  f  g  h " << endl << endl;

#ifdef DEBUG
    // Print pieceArray
	cout << "  ----------------------- " << endl;
	for (int i = BOARD_LENGTH-BORDER_DEPTH-1; i >= BORDER_DEPTH; i--) {
		cout << i-1 << "|";
		for (int j = BORDER_DEPTH; j < BOARD_LENGTH-BORDER_DEPTH; j++) {
			char neg = pieceArray[i][j] == NULL ? '_' : 'P';
			cout << neg << ' ' << "|";
		}
		cout << endl;
	}
	cout << "  ----------------------- " << endl;
	cout << "   a  b  c  d  e  f  g  h " << endl << endl;

#endif
}

bool ChessBoard::is_game_over() {
    // The game is only over when a team has no moves left to make.
	return valid_moves.size() == 0;
}

void ChessBoard::check_integrity() {
	// Checks the integrity of all chessboard components.
	for (auto p : PieceListWhite) {
		if (p->get_team() != WHITE) {
			std::runtime_error("Piece list mismatch, white");
		}
		auto coords = p->get_coords();
		int row = std::get<0>(coords);
		int col = std::get<1>(coords);
		if (board_state[row][col] != p->get_player_piece()) {
			std::runtime_error("Piece board mismatch");
		}
		if (pieceArray[row][col] != p) {
			std::runtime_error("Piece/pieceArray mismatch");
		}
		p->check_integrity();
	}
	std::cout << "Finished checking white list" << std::endl;
	for (auto p : PieceListBlack) {
		if (p->get_team() != BLACK) {
			std::runtime_error("Piece list mismatch, black");
		}
		auto coords = p->get_coords();
		int row = std::get<0>(coords);
		int col = std::get<1>(coords);
		if (board_state[row][col] != p->get_player_piece()) {
			std::runtime_error("Piece board mismatch");
		}
		if (pieceArray[row][col] != p) {
			std::runtime_error("Piece/pieceArray mismatch");
		}
		p->check_integrity();
	}
	std::cout << "Finished checking black list" << std::endl;
	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
            if (board_state[i][j] == 0 && pieceArray[i][j] != NULL) {
                std::runtime_error("Mismatch b/w board and piece");
            }
			if (pieceArray[i][j] != NULL) {
				Piece* thisPiece = pieceArray[i][j];
				if (thisPiece->get_player_piece() != board_state[i][j]) {
					std::runtime_error("Piece in Piece array is incorrect");
				}
                if (std::find(PieceListWhite.begin(), PieceListWhite.end(), thisPiece) == PieceListWhite.end() &&
                    std::find(PieceListBlack.begin(), PieceListBlack.end(), thisPiece) == PieceListBlack.end()) {
                    std::runtime_error("Piece is not in piece list!!");
                }
                if (board_state[i][j] == 0 || board_state[i][j] == Piece::Types::INVALID) {
                    std::runtime_error("In piece array, but not in board state");
                }
			} else {
				if (board_state[i][j] != 0 && board_state[i][j] != Piece::Types::INVALID) {
					std::runtime_error("Board state and Piece array do not match");
				} 
			}
		}
	}
}
