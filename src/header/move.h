#ifndef MOVE_H
#define MOVE_H

#include <tuple>
#include <algorithm>
#include <string>
#include <map>

#include "piece.h"

#ifndef BORDER_DEPTH
#define BORDER_DEPTH 2
#endif

class Move {
public:
    enum MoveType {
        MOVE,
        CASTLE,
        PROMOTE,
    };

    Move();
    Move(MoveType move, std::string from, std::string to, Piece::Types type = Piece::NONE);
    Move(MoveType move, std::pair<int,int> from, std::pair<int,int> to, Piece::Types type = Piece::NONE);
    ~Move();

    inline bool operator ==(const Move& rhs) {
        return
            this->get_move() == rhs.get_move() &&
            this->get_from() == rhs.get_from() &&
            this->get_to() == rhs.get_to() &&
            this->get_type() == rhs.get_type();
    }
    inline void operator =(const Move& rhs) {
        this->move = rhs.get_move();
        this->from = rhs.get_from();
        this->to = rhs.get_to();
        this->type = rhs.get_type();
    }

    MoveType get_move() const;
    std::pair<int,int> get_from() const;
    std::pair<int,int> get_to() const;
    Piece::Types get_type() const;
    std::string get_move_string() const;
private:
    std::map<char,int> charToCol = {
        {'a',2},
        {'b',3},
        {'c',4},
        {'d',5},
        {'e',6},
        {'f',7},
        {'g',8},
        {'h',9}
    };
    Piece::Types type;
    std::pair<int,int> from;
    std::pair<int,int> to;
    MoveType move;
};

#endif
