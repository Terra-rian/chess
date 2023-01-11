#include "app.hpp"

wxIMPLEMENT_APP(ChessApp);

bool ChessApp::OnInit() {
    wxInitAllImageHandlers();

    return true;
}