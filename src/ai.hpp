#pragma once

class Board;
class Piece;

class AI {
    public:
    static void playTurn(Board* board);

    private:
    static Piece* chooseRandomPiece(Board* board);
};