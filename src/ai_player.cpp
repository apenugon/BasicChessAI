#include "ai_player.h"

#define MATERIAL_WEIGHT 10
#define MOBILITY_WEIGHT 1
#define MINIMAX_DEPTH 4

AIPlayer::AIPlayer(std::string team) {
    this->name = "AI " + team + " Player";
    material_scores[Piece::Types::PAWN] = 1;
    material_scores[Piece::Types::KNIGHT] = 3;
    material_scores[Piece::Types::BISHOP] = 3;
    material_scores[Piece::Types::ROOK] = 3;
    material_scores[Piece::Types::QUEEN]=5;
    material_scores[Piece::Types::KING]=0;
    this->team = team[0] == 'W' ? ChessBoard::WHITE : ChessBoard::BLACK;
}

AIPlayer::~AIPlayer() {

}

std::tuple<ChessBoard::MoveType, std::string, std::string> AIPlayer::get_move(ChessBoard* board) {

    std::pair<int,std::tuple<ChessBoard::MoveType, std::pair<int,int>, std::pair<int,int>>>* score_move;
    
    

    
    auto score_move_val = evaluate_node(board, 0, INT_MIN, INT_MAX);
    score_move = &score_move_val;
    
    

    auto move = std::get<1>(*score_move);
    auto move_type = std::get<0>(move);
    auto from = std::get<1>(move);
    auto to = std::get<2>(move);

    std::cout << "from: " << std::get<0>(from) << "," << std::get<1>(from);
    std::cout << " to: " << std::get<0>(to) << "," << std::get<1>(to) << std::endl;

    char colNumToChar[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    std::stringstream ss;
    ss << colNumToChar[std::get<1>(from) - 2] << (char)(std::get<0>(from) - 1 + '0');

    std::string from_str = ss.str();

    std::stringstream to_ss;
    to_ss << colNumToChar[std::get<1>(to) - 2] << (char)(std::get<0>(to) - 1 + '0');
    std::string to_str = to_ss.str();
    std::cout << "AI moving from " << from_str << " to " << to_str << std::endl;

    return std::make_tuple(move_type, from_str, to_str);
}

std::pair<int, std::tuple<ChessBoard::MoveType, std::pair<int,int>, std::pair<int,int>>> AIPlayer::evaluate_node(ChessBoard* board, int depth, int alpha, int beta) {
    if (depth == MINIMAX_DEPTH)
        return std::make_pair(evaluate_board(board), std::make_tuple(ChessBoard::MOVE, std::make_pair(0,0), std::make_pair(0,0)));

    std::map<int, std::tuple<ChessBoard::MoveType, std::pair<int,int>, std::pair<int,int>>> score_to_move;

    for (auto m : board->get_valid_moves()) {
        
        
        ChessBoard* next = board->makeMove(m);
        auto score_move = evaluate_node(next, depth+1, -beta, -alpha);
        int score = std::get<0>(score_move);
       
        alpha = std::max(alpha, score);
        score_to_move[score] = m;
        
        delete next;
        
        if (alpha >= beta)
            break;
    }

    int max_score;
    if (score_to_move.size() > 0)
        max_score = score_to_move.rbegin()->first;
    else
        max_score = INT_MIN;
    return std::make_pair(-max_score, score_to_move[max_score]);
}

int AIPlayer::evaluate_board(ChessBoard* board) {
    // Material
    int material = 0;
    for (auto p : board->getPieceList(board->get_team())) {
        material+=this->material_scores[p->typeOf()];
    }

    for (auto p : board->getPieceList(board->get_opposing_team())) {
        material-=2*(this->material_scores[p->typeOf()]);
    }

    int mobility = board->num_available_moves();
    int squares_in_danger = 0;

    #pragma omp parallel for reduction(+:squares_in_danger)
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            squares_in_danger += (int)(board->square_in_danger(2+i,2+j));
        }
    }

    return MATERIAL_WEIGHT * material + MOBILITY_WEIGHT * mobility - squares_in_danger - 10 * board->isInCheck(board->get_team());
}
