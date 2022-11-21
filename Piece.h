#ifndef PIECE_H
#define PIECE_H
#include<string>
#include<tuple>
#include<iostream>
#include <math.h>  
#include<vector>
class Piece{
    private:
        int id;
        int width;
        int height;
        std::string name;
        std::vector<std::tuple<int, int>> coords;
    public:
        Piece*clone();
        const float PI=3.14159265f;
        const char* PIECE_SQUARE = "Square";
        const char* PIECE_T = "T";
        const char* PIECE_L = "L";
        const char* PIECE_INVERSE_L = "Inverse_L";
        const char* PIECE_LINE = "Line";
        void rotate(bool clock_wise);
        Piece(){};
        Piece(std::string name);
        ~Piece();
        std::vector<std::tuple<int, int>> get_coords() {return coords;}
        std::string get_name() {return name;}
        int get_id() {return id;}
        int get_width() {return width;}
        int get_height(){return height;}
};
#endif