#include "board.hpp"
#include "ai.hpp"
#include "cell.hpp"
#include "piece.hpp"

Board::Board() : turn{"white"}, gameFinished{false} {
    initPieces();
    initCells();
}

const std::unordered_map<std::string, Piece*>& Board::getPiecesMap() {
    return pieces;
}

Piece* Board::getPieceAt(int cellX, int cellY) {
    return cells[cellX][cellY]->getPiece();
}

Piece* Board::getSelectedPiece() {
    return selectedPiece;
}

void Board::setSelectedPiece(Piece* piece) {
    selectedPiece = piece;
}

Cell* Board::getCellAt(int cellX, int cellY) {
    return cells[cellX][cellY];
}

std::string Board::getTurn() {
    return turn;
}

void Board::switchTurn() {
    if(gameFinished) {
        return;
    }

    turn = turn == "white" ? "black" : "white";
    if(turn == "black" && enemyIsAI) {
        AI::playTurn(this);
    }
}

void Board::setGameFinished(bool gameFinished) {
    this->gameFinished = gameFinished;

    wxMessageBox(wxString("Game ended, " + turn + " wins."));
}

bool Board::isGameFinished() {
    return gameFinished;
}

void Board::eraseAllIllumination() {
    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            getCellAt(x, y)->turnOff();
        }
    }
}

void Board::setEnemyIsAI(bool enemyIsAI) {
    this->enemyIsAI = enemyIsAI;
}

bool Board::isThereEnemy(int cellX, int cellY) {
    return cells[cellX][cellY]->getPiece() != nullptr && cells[cellX][cellY]->getPiece()->getColor() != turn;
}

bool Board::isThereAlly(int cellX, int cellY) {
    return cells[cellX][cellY]->getPiece() != nullptr && cells[cellX][cellY]->getPiece()->getColor() == turn;
}

bool Board::isTherePiece(int cellX, int cellY) {
    return cells[cellX][cellY]->hasPiece();
}

void Board::initPieces() {
    std::string ids[]{"Rook_0", "Knight_0", "Bishop_0", "Queen", "King", "Bishop_1", "Knight_1", "Rook_1"};

    for(int x = 0; x < 8; x++) {
        wxImage image("images/black_pawn.png");
        Piece *pawn = new Pawn(x, 1, wxBitmap(image), "B_Pawn_" + std::to_string(x));

        pieces[pawn->getId()] = pawn;
    }

    for(int x = 0; x < 8; x++) {
        wxImage image("images/white_pawn.png");
        Piece *pawn = new Pawn(x, 6, wxBitmap(image), "W_Pawn_" + std::to_string(x));

        pieces[pawn->getId()] = pawn;
    }

    std::string colors[] = {"W", "B"};
    for(int x = 0; x < 8; x++) {
        for(auto& color : colors) {
            Piece *piece;
            std::string imageName = color == "W" ? "white" : "black";

            switch(ids[x][0]) {
                case 'R':
                    imageName += "_rook.png";
                    piece = new Rook(x, color == "W" ? 7 : 0, wxBitmap(wxImage("images/" + imageName)), color + "_" + ids[x]);
                    break;

                case 'K':
                    if(ids[x][1] == 'i') {
                        imageName += "_king.png";
                        piece = new King(x, color == "W" ? 7 : 0, wxBitmap(wxImage("images/" + imageName)), color + "_" + ids[x]);
                    } else {
                        imageName += "_knight.png";
                        piece = new Knight(x, color == "W" ? 7 : 0, wxBitmap(wxImage("images/" + imageName)), color + "_" + ids[x]);
                    }
                    break;

                case 'B':
                    imageName += "_bishop.png";
                    piece = new Bishop(x, color == "W" ? 7 : 0, wxBitmap(wxImage("images/" + imageName)), color + "_" + ids[x]);
                    break;

                case 'Q':
                    imageName += "_queen.png";
                    piece = new Queen(x, color == "W" ? 7 : 0, wxBitmap(wxImage("images/" + imageName)), color + "_" + ids[x]);
                    break;
            }

            pieces[piece->getId()] = piece;
        }
    }
}

void Board::initCells() {
    std::vector<Cell*> empty(8);

    for(int i = 0; i < 8; i++) {
        cells.push_back(empty);
        for(int j = 0; j < 8; j++) {
            cells[i][j] = new Cell();
        }
    }

    for(auto& it : pieces) {
        Piece *piece = it.second;
        int x = piece->getCellX();
        int y = piece->getCellY();

        cells[x][y] = new Cell();
        cells[x][y]->setPiece(piece);
    }
}