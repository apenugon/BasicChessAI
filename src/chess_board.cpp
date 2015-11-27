#include "chess_board.h"

using namespace std;

bool ChessBoard::isPieceHere(int row, int col) {
    return board_state[row][col] != 0;
}

std::vector<Move> ChessBoard::get_valid_moves() {
    return this->valid_moves;
}

int ChessBoard::num_available_moves() {
    return valid_moves.size();
}

ChessBoard::Teams ChessBoard::get_team() {
    return current_team;
}

ChessBoard::Teams ChessBoard::get_opposing_team() {
    return current_team == WHITE ? BLACK : WHITE;
}

std::vector<Piece*> ChessBoard::getPieceList(ChessBoard::Teams team) {
    if (team == WHITE)
        return this->PieceListWhite;
    else
        return this->PieceListBlack;
}

void ChessBoard::print_available_moves() {
	std::string team = current_team == WHITE ? "WHITE" : "BLACK";
	std::cout << "Available Moves for " << team << std::endl;
	for (auto m : valid_moves) {
		std::string move = m.get_move_string();
		pair<int,int> from = m.get_from();
		pair<int,int> to = m.get_to();

		int from_row;
		int from_col;
        tie(from_row, from_col) = from;
		int to_row;
		int to_col;
        tie(to_row, to_col) = to;
		std::cout << move << " " << colNumToChar[from_col-BORDER_DEPTH]  << from_row-BORDER_DEPTH+1 << " " << colNumToChar[to_col-BORDER_DEPTH] << to_row-BORDER_DEPTH+1;

        switch (m.get_type()) {
        case Piece::ROOK:
            cout << " R";
            break;
        case Piece::KNIGHT:
            cout << " N";
            break;
        case Piece::BISHOP:
            cout << " B";
            break;
        case Piece::QUEEN:
            cout << " Q";
        default:
            break;
        }
        cout << endl;
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
		for (int i = 0; i < PieceListBlack.size(); i++) {
			isInCheck |= (PieceListBlack[i])->threatens_king();
		}
	else
		for (int i = 0; i <  PieceListWhite.size(); i++) {
			isInCheck |= (PieceListWhite[i])->threatens_king();
		}
	return isInCheck;
	// Check to see if you run into pieces in any direction, 
}

void ChessBoard::move_piece(Piece *piece, 
				int toRow, 
				int toCol, 
				int new_board_state[][BOARD_LENGTH], 
				Piece* new_pieceArray[][BOARD_LENGTH]) {
    int orig_row;
    int orig_col;
	tie(orig_row, orig_col) = piece->get_coords();
	new_board_state[orig_row][orig_col] = Piece::Types::NONE;
	new_board_state[toRow][toCol] = board_state[orig_row][orig_col];
	new_pieceArray[orig_row][orig_col] = NULL;
	new_pieceArray[toRow][toCol] = piece;
	piece->move(toRow, toCol, new_board_state);
}

ChessBoard* ChessBoard::makeMove(Move move, bool is_valid, bool do_generate_moves) {
	auto in_bounds = [] (std::pair<int,int> in_coords) {
        int row;
        int col;
        tie(row, col) = in_coords;
        return (unsigned)row < BOARD_LENGTH &&
               (unsigned)col < BOARD_LENGTH;};

	// Validate move
	if (!is_valid) {
		if (!in_bounds(move.get_from()) || !in_bounds(move.get_to()))
        	return NULL;
		for (int i = 0; i < valid_moves.size(); i++) {
			is_valid |= valid_moves[i] == move;
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

    int fromRow;
    int fromCol;
    tie(fromRow, fromCol) = move.get_from();

    int toRow;
    int toCol;
    tie(toRow, toCol) = move.get_to();

    auto move_type = move.get_move();

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
				if ((move_type == Move::MOVE || move_type == Move::PROMOTE) && i == toRow && j == toCol) {
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

    auto piece_to_move = new_pieceArray[fromRow][fromCol];
	switch (move_type) {
        case Move::CASTLE: {
			auto king = new_pieceArray[fromRow][fromCol];
			auto rook = new_pieceArray[toRow][toCol];
			// Determine if this is a left castle or a right castle
			if (toCol < fromCol) {
				// Left Castle
				move_piece(king, fromRow, fromCol-2, new_board_state, new_pieceArray);
				move_piece(rook, toRow, toCol+3, new_board_state, new_pieceArray);
			} else {
				// Right Castle
				move_piece(king, fromRow, fromCol+2, new_board_state, new_pieceArray);
				move_piece(rook, toRow, toCol-2, new_board_state, new_pieceArray);
			}
			break;
		}
        case Move::PROMOTE: {
            move_piece(piece_to_move, toRow, toCol, new_board_state, new_pieceArray);
            piece_to_move->promote(move.get_type());
            // Update new board state
            new_board_state[toRow][toCol] = (int)piece_to_move->get_player_piece();
            break;
        }
		default:
			// Just do a normal move
			move_piece(piece_to_move, toRow, toCol, new_board_state, new_pieceArray);
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
	for (int i = 0; i < PieceListBlack.size(); i++) {
        auto p = PieceListBlack[i];
		p->generate_valid_moves(board_state);
	}
	for (int i = 0; i < PieceListWhite.size(); i++) {
        auto p = PieceListWhite[i];
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

    std::vector<Move> local_valid_moves;


    #pragma omp declare reduction (merge : std::vector<Move> : omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end())) 
	// Generate move sets for each piece, add them to the actual valid move list
    #pragma omp parallel for reduction(merge : local_valid_moves)
	for (std::vector<Piece*>::iterator p = useVector->begin(); p < useVector->end(); ++p) {
		for (auto piece_move : (*p)->get_valid_moves()) {	
#ifdef DEBUG
            if (piece_move == (*p)->get_coords()) {
                cout << "Alert! Moving to same piece!!" << endl;
            }
#endif
            if ((*p)->get_can_promote()) {
                // Need to add all possible ways to promote pawn
                local_valid_moves.push_back(Move(Move::PROMOTE, (*p)->get_coords(), piece_move, Piece::ROOK));
                local_valid_moves.push_back(Move(Move::PROMOTE, (*p)->get_coords(), piece_move, Piece::BISHOP));
                local_valid_moves.push_back(Move(Move::PROMOTE, (*p)->get_coords(), piece_move, Piece::KNIGHT));
                local_valid_moves.push_back(Move(Move::PROMOTE, (*p)->get_coords(), piece_move, Piece::QUEEN));

            } else {
			    local_valid_moves.push_back(Move(Move::MOVE, (*p)->get_coords(), piece_move));
            }
		}
	}

    this->valid_moves = local_valid_moves;

    // Generate En Passent moves
    for (auto p : (*useVector)) {
        if (p->typeOf() == Piece::Types::PAWN) {
            // Now check for En Passent conditions
            int row;
            int col;
            tie(row, col) = p->get_coords();
            // Left Side:
            if (pieceArray[row][col-1] != NULL &&
                pieceArray[row][col-1]->is_special_move_used() == true &&
                pieceArray[row][col-1]->get_same_pos_counter() == 1 &&
                pieceArray[row][col-1]->get_move_counter() == 1 &&
                pieceArray[row][col-1]->get_team() != this->current_team) {

                valid_moves.push_back(Move(Move::MOVE, p->get_coords(), make_pair(row-current_team, col-1)));
            }
            // Right
            if (pieceArray[row][col+1] != NULL &&
                pieceArray[row][col+1]->is_special_move_used() == true &&
                pieceArray[row][col+1]->get_same_pos_counter() == 1 &&
                pieceArray[row][col+1]->get_move_counter() == 1 &&
                pieceArray[row][col+1]->get_team() != this->current_team) {

                valid_moves.push_back(Move(Move::MOVE, p->get_coords(), make_pair(row-current_team, col+1)));
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
				valid_moves.push_back(Move(Move::CASTLE, coords, rook->get_coords()));
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
				valid_moves.push_back(Move(Move::CASTLE, king->get_coords(), rook->get_coords()));
			}
		}

	}

#ifdef DEBUG

    for (auto m : valid_moves) {
        if (m.get_from() == m.get_to()) {
            cout << "Alert! Moving to same piece!!" << endl;
        }
    }
#endif

	//Are there pieces in between?
	auto filter_function = [this] (Move move) {
		// Make move, determine if king in check
		#ifdef DEBUG
		int a = std::get<0>(move.get_from());
		int b = std::get<1>(move.get_from());
		int c = std::get<0>(move.get_to());
		int d = std::get<1>(move.get_to());
		std::cout << "Filtering move " << a << "," << b << " " << c << "," <<  d << std::endl;
		#endif
		ChessBoard* nextBoard = this->makeMove(move, true, false);
		bool is_invalid_move = nextBoard->isInCheck(current_team);
		delete nextBoard;
		return is_invalid_move;
	};

    std::vector<Move> surviving_moves;

    #pragma omp parallel for reduction(merge : surviving_moves)
    for (int i = 0; i < valid_moves.size(); i++) {
        auto m = valid_moves[i];
        if (!filter_function(m)) {
            surviving_moves.push_back(m);
        }
    }

    this->valid_moves = surviving_moves;
}

// Creates a new chessboard, set up with the next board state.
ChessBoard::ChessBoard(int board_state[][BOARD_LENGTH], 
	std::vector<Piece*> PieceListWhite, 
	std::vector<Piece*> PieceListBlack, 
	Piece* pieceArray[][BOARD_LENGTH],
	Teams team,
	bool do_generate_moves) {	

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

int ChessBoard::winner() {
    if (isInCheck(WHITE))
        return BLACK;
    else if (isInCheck(BLACK))
        return WHITE;
    else
        return 0;
}

bool ChessBoard::is_game_over() {
    // The game is only over when a team has no moves left to make, 
    // or there are only kings left.
	return valid_moves.size() == 0 ||
        (PieceListWhite.size() == 1 && PieceListBlack.size() == 1);
}

void ChessBoard::check_integrity() {
	// Checks the integrity of all chessboard components.
	for (auto p : PieceListWhite) {
		if (p->get_team() != WHITE) {
			std::runtime_error("Piece list mismatch, white");
		}

		int row;
		int col;
        tie(row, col) = p->get_coords();
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

		int row;
		int col;
        tie(row, col) = p->get_coords();
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
