#pragma once

#include <wx/wx.h>

class ChessApp : public wxApp {
    public:
    bool OnInit() wxOVERRIDE;
};

wxDECLARE_APP(ChessApp);