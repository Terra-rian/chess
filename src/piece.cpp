#include "piece.hpp"
#include "board.hpp"
#include "cell.hpp"

Piece::Piece(int cellX, int cellY, wxBitmap image, std::string id) {
    this->cellX = cellX;
    this->cellY = cellY;
    this->id = id;

    this->image = image;
    this->color = id[0] == 'W' ? "white" : "black";

    this->alive = true;
}

std::string Piece::getId() {
    return id;
}

int Piece::getCellX() {
    return cellX;
}

int Piece::getCellY() {
    return cellY;
}

void Piece::setCellX(int x) {
    cellX = x;
}

void Piece::setCellY(int y) {
    cellY = y;
}

wxBitmap Piece::getImage() {
    return image;
}

std::string Piece::getColor() {
    return color;
}

bool Piece::isAlive() {
    return alive;
}

void Piece::setAlive(bool isAlive) {
    alive = isAlive;
}

bool Piece::canMove(Board* board) {
    illuminatePaths(board);

    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            if(board->getCellAt(x, y)->isIlluminated()) {
                board->eraseAllIllumination();

                return true;
            }
        }
    }

    board->eraseAllIllumination();

    return false;
}

void Piece::move(int targetX, int targetY, Board* board) {
    if(board->isTherePiece(targetX, targetY)) {
        board->getPieceAt(targetX, targetY)->setAlive(false);

        if(board->getPieceAt(targetX, targetY)->getId().find("King") != std::string::npos) {
            board->setGameFinished(true);
        }
    }

    board->getCellAt(cellX, cellY)->setPiece(nullptr);

    cellX = targetX;
    cellY = targetY;

    board->getCellAt(targetX, targetY)->setPiece(this);
}

Pawn::Pawn(int cellX, int cellY, wxBitmap image, std::string id) : Piece(cellX, cellY, image, id) {}

void Pawn::illuminatePaths(Board* board) {
    short sign = 1;

    if(color == "black") {
        sign = -1;
    }

    std::pair<int, int> offsets[3] = {{-1, -1 * sign}, {0, -1 * sign}, {1, -1 * sign}};
    for(auto offset : offsets) {
        if(cellX + offset.first >= 0 && cellX + offset.first < 8 && cellY + offset.second >= 0 && cellY + offset.second < 8) {
            if(offset.first == 0) {
                if(!board->isTherePiece(cellX, cellY + offset.second)) {
                    board->getCellAt(cellX, cellY + offset.second)->turnOn();
                }
            } else {
                if(board->isThereEnemy(cellX + offset.first, cellY + offset.second)) {
                    board->getCellAt(cellX + offset.first, cellY + offset.second)->turnOn();
                }
            }
        }
    }

    if((cellY == 6 && color == "white") || (cellY == 1 && color == "black")) {
        board->getCellAt(cellX, cellY - 2 * sign)->turnOn();
    }
}

Rook::Rook(int cellX, int cellY, wxBitmap image, std::string id) : Piece(cellX, cellY, image, id) {}

void Rook::illuminatePaths(Board* board) {
    int x, y;

    for(y = cellY - 1; y >= 0 && !board->isTherePiece(cellX, y); y--) {
        board->getCellAt(cellX, y)->turnOn();
    }

    if(y >= 0 && board->isThereEnemy(cellX, y)) {
        board->getCellAt(cellX, y)->turnOn();
    }

    for(y = cellY + 1; y < 8 && !board->isTherePiece(cellX, y); y++) {
        board->getCellAt(cellX, y)->turnOn();
    }

    if(y < 8 && board->isThereEnemy(cellX, y)) {
        board->getCellAt(cellX, y)->turnOn();
    }

    for(x = cellX - 1; x >= 0 && !board->isTherePiece(x, cellY); x--) {
        board->getCellAt(x, cellY)->turnOn();
    }

    if(x >= 0 && board->isThereEnemy(x, cellY)) {
        board->getCellAt(x, cellY)->turnOn();
    }

    for(x = cellX + 1; x < 8 && !board->isTherePiece(x, cellY); x++) {
        board->getCellAt(x, cellY)->turnOn();
    }

    if(x < 8 && board->isThereEnemy(x, cellY)) {
        board->getCellAt(x, cellY)->turnOn();
    }
}

Knight::Knight(int cellX, int cellY, wxBitmap image, std::string id) : Piece(cellX, cellY, image, id) {}

void Knight::illuminatePaths(Board* board) {
    std::pair<int, int> offsets[8] = {{-1, -2}, {-1, 2}, {-2, 1}, {-2, -1}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
    for(auto offset : offsets) {
        if(cellX + offset.first >= 0 && cellX + offset.first < 8 && cellY + offset.second >= 0 && cellY + offset.second < 8 && !board->isThereAlly(cellX + offset.first, cellY + offset.second)) {
            board->getCellAt(cellX + offset.first, cellY + offset.second)->turnOn();
        }
    }
}

Bishop::Bishop(int cellX, int cellY, wxBitmap image, std::string id) : Piece(cellX, cellY, image, id) {}

void Bishop::illuminatePaths(Board* board) {
    int x, y;

    for(x = cellX-1, y = cellY - 1; x >= 0 && y >= 0 && !board->isTherePiece(x, y); x--, y--) {
        board->getCellAt(x, y)->turnOn();
    }

    if(x >= 0 && y >= 0 && board->isThereEnemy(x, y)) {
        board->getCellAt(x, y)->turnOn();
    }

    for(x = cellX+1, y = cellY - 1; x < 8 && y >= 0 && !board->isTherePiece(x, y); x++, y--) {
        board->getCellAt(x, y)->turnOn();
    }

    if(x < 8 && y >= 0 && board->isThereEnemy(x, y)) {
        board->getCellAt(x, y)->turnOn();
    }

    for(x = cellX-1, y = cellY + 1; x >= 0 && y < 8 && !board->isTherePiece(x, y); x--, y++) {
        board->getCellAt(x, y)->turnOn();
    }

    if(x >= 0 && y < 8 && board->isThereEnemy(x, y)) {
        board->getCellAt(x, y)->turnOn();
    }

    for(x = cellX + 1, y = cellY + 1; x < 8 && y < 8 && !board->isTherePiece(x, y); x++, y++) {
        board->getCellAt(x, y)->turnOn();
    }

    if(x < 8 && y < 8 && board->isThereEnemy(x, y)) {
        board->getCellAt(x, y)->turnOn();
    }
}

Queen::Queen(int cellX, int cellY, wxBitmap image, std::string id) : Piece(cellX, cellY, image, id) {}

void Queen::illuminatePaths(Board* board) {
    int x, y;

    for(x = cellX - 1, y = cellY - 1; x >= 0 && y >= 0 && !board->isTherePiece(x, y); x--, y--) {
        board->getCellAt(x, y)->turnOn();
    }

    if(x >= 0 && y >= 0 && board->isThereEnemy(x, y)) {
        board->getCellAt(x, y)->turnOn();
    }

    for(x = cellX + 1, y = cellY - 1; x < 8 && y >= 0 && !board->isTherePiece(x, y); x++, y--) {
        board->getCellAt(x, y)->turnOn();
    }

    if(x < 8 && y >= 0 && board->isThereEnemy(x, y)) {
        board->getCellAt(x, y)->turnOn();
    }

    for(x = cellX - 1, y = cellY + 1; x >= 0 && y < 8 && !board->isTherePiece(x, y); x--, y++) {
        board->getCellAt(x, y)->turnOn();
    }

    if(x >= 0 && y < 8 && board->isThereEnemy(x, y)) {
        board->getCellAt(x, y)->turnOn();
    }

    for(x = cellX + 1, y = cellY + 1; x < 8 && y < 8 && !board->isTherePiece(x, y); x++, y++) {
        board->getCellAt(x, y)->turnOn();
    }

    if(x < 8 && y < 8 && board->isThereEnemy(x, y)) {
        board->getCellAt(x, y)->turnOn();
    }

    for(y = cellY - 1; y >= 0 && !board->isTherePiece(cellX, y); y--) {
        board->getCellAt(cellX, y)->turnOn();
    }

    if(y >= 0 && board->isThereEnemy(cellX, y)) {
        board->getCellAt(cellX, y)->turnOn();
    }

    for(y = cellY + 1; y < 8 && !board->isTherePiece(cellX, y); y++) {
        board->getCellAt(cellX, y)->turnOn();
    }

    if(y < 8 && board->isThereEnemy(cellX, y)) {
        board->getCellAt(cellX, y)->turnOn();
    }

    for(x = cellX - 1; x >= 0 && !board->isTherePiece(x, cellY); x--) {
        board->getCellAt(x, cellY)->turnOn();
    }

    if(x >= 0 && board->isThereEnemy(x, cellY)) {
        board->getCellAt(x, cellY)->turnOn();
    }

    for(x = cellX + 1; x < 8 && !board->isTherePiece(x, cellY); x++) {
        board->getCellAt(x, cellY)->turnOn();
    }

    if(x < 8 && board->isThereEnemy(x, cellY)) {
        board->getCellAt(x, cellY)->turnOn();
    }
}

King::King(int cellX, int cellY, wxBitmap image, std::string id) : Piece(cellX, cellY, image, id) {}

void King::illuminatePaths(Board* board) {
    std::pair<int, int> offsets[8] = {{-1, -1}, {-1, 1}, {-1, 0}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    for(auto offset : offsets) {
        if(cellX + offset.first >= 0 && cellX + offset.first < 8 && cellY + offset.second >= 0 && cellY + offset.second < 8 && !board->isThereAlly(cellX + offset.first, cellY + offset.second)) {
            board->getCellAt(cellX + offset.first, cellY + offset.second)->turnOn();
        }
    }
}