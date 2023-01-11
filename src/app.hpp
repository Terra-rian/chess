#pragma once

#include <wx/wx.h>
#include "frame.hpp"

class ChessApp : public wxApp {
    public:
    bool OnInit() wxOVERRIDE;
};

wxDECLARE_APP(ChessApp);