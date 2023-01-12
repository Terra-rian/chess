#include "app.hpp"
#include "frame.hpp"

IMPLEMENT_APP(ChessApp)

bool ChessApp::OnInit() {
    wxInitAllImageHandlers();

    ChessFrame *chessFrame = new ChessFrame(wxT("Chess"));
    chessFrame->SetIcon(wxICON(aaaa));
    chessFrame->Show(true);

    return true;
}