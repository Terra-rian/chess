#include "panel.hpp"
#include "board.hpp"
#include "cell.hpp"
#include "piece.hpp"

ChessPanel::ChessPanel(wxFrame *parent, Board *board) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE), board{board}, enemySelected{false} {
    wxWindow::SetBackgroundStyle(wxBG_STYLE_PAINT);

    Connect(wxEVT_PAINT, wxPaintEventHandler(ChessPanel::OnPaint));
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ChessPanel::OnLeftMouseDown));
}

void ChessPanel::OnPaint(wxPaintEvent& WXUNUSED(event)) {
    wxBufferedPaintDC dc(this);
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    clearBuffer(gc);

    if(!enemySelected) {
        drawSelectionScreen(gc);
    } else {
        drawBoard(gc);
        drawPieces(gc);
    }

    delete gc;
}

void ChessPanel::clearBuffer(wxGraphicsContext *gc) {
    const wxColor white = wxColor(255, 255, 255);

    gc->SetPen(*wxTRANSPARENT_PEN);
    gc->SetBrush(wxBrush(white));
    gc->DrawRectangle(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight());
}

void ChessPanel::OnLeftMouseDown(wxMouseEvent& event) {
    if(board->isGameFinished()) {
        return;
    }

    if(!enemySelected) {
        handleScreenSelection(event.GetPosition());
        return;
    }

    auto pos = event.GetPosition();

    const int cellLenX = GetClientSize().GetWidth() / 8;
    const int cellLenY = GetClientSize().GetHeight() / 8;

    int cellX = static_cast<int>(pos.x / cellLenX);
    int cellY = static_cast<int>(pos.y / cellLenY);

    Cell* selectedCell = board->getCellAt(cellX, cellY);

    if(board->isThereAlly(cellX, cellY)) {
        board->eraseAllIllumination();

        auto piece = selectedCell->getPiece();
        piece->illuminatePaths(board);
        board->setSelectedPiece(piece);

        wxPanel::Refresh();
    } else if(selectedCell->isIlluminated()) {
        board->getSelectedPiece()->move(cellX, cellY, board);
        board->eraseAllIllumination();
        board->setSelectedPiece(nullptr);
        board->switchTurn();

        wxPanel::Refresh();
    } else {
        board->eraseAllIllumination();
        board->setSelectedPiece(nullptr);

        wxPanel::Refresh();
    }
}

void ChessPanel::drawBoard(wxGraphicsContext* gc) {
    const wxColor white = wxColor(238, 238, 210);
    const wxColor green = wxColor(118, 150, 86);
    const wxColor yellow = wxColor(255, 255, 128, 220);

    gc->SetPen(*wxTRANSPARENT_PEN);

    const int cellLenX = GetClientSize().GetWidth() / 8;
    const int cellLenY = GetClientSize().GetHeight() / 8;

    for(int y = 0; y < 8; y++) {
        for(int x = 0; x < 8; x++) {
            if((x + y) % 2 == 0) {
                gc->SetBrush(wxBrush(white));
            } else {
                gc->SetBrush(wxBrush(green));
            }

            gc->DrawRectangle(x * cellLenX, y * cellLenY, cellLenX, cellLenY);

            if(board->getCellAt(x, y)->isIlluminated()) {
                gc->SetBrush(wxBrush(yellow));
                gc->DrawRectangle(x * cellLenX, y * cellLenY, cellLenX, cellLenY);
            }
        }
    }
}

void ChessPanel::drawPieces(wxGraphicsContext* gc) {
    const int cellLenX = GetClientSize().GetWidth() / 8;
    const int cellLenY = GetClientSize().GetHeight() / 8;

    auto piecesMap = board->getPiecesMap();
    for(auto& it : piecesMap) {
        Piece *piece = it.second;
        if(piece->isAlive()) {
            gc->DrawBitmap(piece->getImage(), piece->getCellX() * cellLenX + 2, piece->getCellY() * cellLenY, piece->getImage().GetWidth(), piece->getImage().GetHeight());
        }
    }
}

void ChessPanel::drawSelectionScreen(wxGraphicsContext* gc) {
    const wxColor white = wxColor(255, 255, 255);
    const wxColor black = wxColor(0, 0, 0);

    wxPen blackPen(black, 10);
    gc->SetPen(blackPen);

    gc->DrawRectangle(160, 150, 300, 100);
    gc->DrawRectangle(160, 350, 300, 100);
    // gc->SetFont(wxFont(12, wxFONTFAMILY_DECORATIVE, wxNORMAL, wxNORMAL), black);

    wxFont f = *wxNORMAL_FONT;
    f.SetPointSize(25);

    gc->SetFont(f, black);
    gc->DrawText(wxString("Select Enemy:", 20), 200, 50);
    gc->DrawText(wxString("Lazy AI", 20), 250, 170);
    gc->DrawText(wxString("Another Human", 20), 200, 370);
}

void ChessPanel::handleScreenSelection(const wxPoint& point) {
    if(point.x >= 160 && point.x <= 460) {
        if(point.y >= 150 && point.y <= 250) {
            enemySelected = true;
            board->setEnemyIsAI(true);
        }

        if(point.y >= 350 && point.y <= 450) {
            enemySelected = true;
            board->setEnemyIsAI(false);
        }
    }

    wxPanel::Refresh();
}