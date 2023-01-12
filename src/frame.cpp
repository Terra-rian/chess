#include "frame.hpp"
#include "board.hpp"
#include "panel.hpp"

ChessFrame::ChessFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(640, 640)) {
    Centre();

    Board *board = new Board();
    ChessPanel *chessPanel = new ChessPanel(this, board);

    chessPanel->Show(true);
}