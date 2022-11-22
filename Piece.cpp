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
        coords.push_back(std::make_tuple(0, -1));
        coords.push_back(std::make_tuple(0, 0));
        coords.push_back(std::make_tuple(0, 1));
        coords.push_back(std::make_tuple(1, 1));
    }
    if(name == PIECE_INVERSE_L){
        id = 3;
        width = 2;
        height = 3;
        this->name = PIECE_INVERSE_L;
        coords.push_back(std::make_tuple(0, -1));
        coords.push_back(std::make_tuple(0, 0));
        coords.push_back(std::make_tuple(0, 1));
        coords.push_back(std::make_tuple(-1, 1));
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


void Piece::rotate(bool clock_wise){
    if(id != 0){
        int m = clock_wise ? 1:-1;
        float a = 90.0*PI/180;
        for(int i=0; i<coords.size(); i++){
            int x = std::get<0>(coords[i]);
            int y = std::get<1>(coords[i]);
            float new_x = y * (-1);
            float new_y = x * 1;
            x = new_x;
            y = new_y;
            coords[i] = std::make_tuple(x, y);
            int t = width;
            width = height;
            height = t;
        }
    }

}

Piece* Piece::clone(){
    Piece*t = new Piece(this->name);
    return t;
}