#include "Piece.h"
Piece::Piece(std::string name){
    if(name == PIECE_SQUARE){
        id = 0;
        this->name = PIECE_SQUARE;
        width = 2;
        height = 2;
        coords.push_back(std::make_tuple(0, 0));
        coords.push_back(std::make_tuple(-1, 0));
        coords.push_back(std::make_tuple(-1, -1));
        coords.push_back(std::make_tuple(0, -1));
    }
    if(name == PIECE_LINE){
        id = 1;
        this->name = PIECE_LINE;
        width = 4;
        height = 1;
        coords.push_back(std::make_tuple(-1, 0));
        coords.push_back(std::make_tuple(-0, 0));
        coords.push_back(std::make_tuple(1, 0));
        coords.push_back(std::make_tuple(2, 0));
    }
    if(name == PIECE_L){
        id = 2;
        this->name = PIECE_L;
        width = 2;
        height = 3;
        coords.push_back(std::make_tuple(0, -2));
        coords.push_back(std::make_tuple(0, -1));
        coords.push_back(std::make_tuple(0, 0));
        coords.push_back(std::make_tuple(1, 0));
    }
    if(name == PIECE_INVERSE_L){
        id = 3;
        width = 2;
        height = 3;
        this->name = PIECE_INVERSE_L;
        coords.push_back(std::make_tuple(0, -2));
        coords.push_back(std::make_tuple(0, -1));
        coords.push_back(std::make_tuple(0, 0));
        coords.push_back(std::make_tuple(-1, 0));
    }
    if(name == PIECE_T){
        id = 4;
        width = 3;
        height = 2;
        this->name = PIECE_T;
        coords.push_back(std::make_tuple(-1, 0));
        coords.push_back(std::make_tuple(0, 0));
        coords.push_back(std::make_tuple(1, 0));
        coords.push_back(std::make_tuple(0, -1));
    }
}
Piece::~Piece(){
    
}

Piece* Piece::clone(){
    Piece*t = new Piece(this->name);
    return t;
}