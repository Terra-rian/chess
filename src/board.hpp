#pragma once

#include <wx/wx.h>
#include <wx/msgdlg.h>
#include <unordered_map>

class Piece;
class Cell;

class Board {
    public:
    Board();

    Piece* getPieceAt(int x, int y);
    Cell* getCellAt(int cellX, int cellY);
    const std::unordered_map<std::string, Piece*>& getPiecesMap();
    
    std::string getTurn();
    void switchTurn();

    bool isThereEnemy(int cellX, int cellY);
    bool isThereAlly(int cellX, int cellY);
    bool isTherePiece(int cellX, int cellY);

    Piece* getSelectedPiece();
    void setSelectedPiece(Piece* piece);

    void setGameFinished(bool gameFinished);
    bool isGameFinished();

    void setEnemyIsAI(bool enemyIsAI);
    void eraseAllIllumination();

    std::string turn;

    private:
    void initPieces();
    void initCells();

    std::unordered_map<std::string, Piece*> pieces;
    std::vector<std::vector<Cell*>> cells;

    Piece* selectedPiece;

    bool enemyIsAI;
    bool gameFinished;
};