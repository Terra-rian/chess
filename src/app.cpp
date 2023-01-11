#include "app.hpp"

wxIMPLEMENT_APP(ChessApp);

bool ChessApp::OnInit() {
    wxInitAllImageHandlers();

    ChessFrame* frame = new ChessFrame(wxT("Chess"));
    frame->SetIcon(wxICON(aaaa));
    frame->Show(true);

    return true;
}