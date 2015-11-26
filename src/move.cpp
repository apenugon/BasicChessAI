#include "move.h"

using namespace std;

Move::Move() {
    // Only for use because of map
    // WARNING - nothing is instantiated here
}

Move::Move(MoveType move, string from, string to, Piece::Types type) {
    this->move = move;
    this->type = type;
    this->from = make_pair(from[1] - '0' + BORDER_DEPTH - 1, charToCol[from[0]]);
    this->to = make_pair(to[1]-'0'+BORDER_DEPTH-1, charToCol[to[0]]);
}

Move::Move(MoveType move, pair<int,int> from, pair<int,int> to, Piece::Types type) {
    this->move = move;
    this->from = from;
    this->to = to;
this->type = type;
}

Move::~Move() {

}

Move::MoveType Move::get_move() const {
    return this->move;
}

pair<int,int> Move::get_from() const {
    return this->from;
}

pair<int,int> Move::get_to() const {
    return this->to;
}

Piece::Types Move::get_type() const {
    return type;
}

string Move::get_move_string() const {
    switch(this->move) {
    case MOVE:
        return "MOVE";
    case CASTLE:
        return "CASTLE";
    case PROMOTE:
        return "PROMOTE";
    }
}
