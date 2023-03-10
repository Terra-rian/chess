#pragma once

class Piece;

class Cell {
    public:
    Cell();

    bool isIlluminated();

    void turnOff();
    void turnOn();

    Piece* getPiece();
    void setPiece(Piece* piece);
    bool hasPiece();

    private:
    Piece* piece;
    bool illuminated;
};