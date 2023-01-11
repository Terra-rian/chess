#include "frame.hpp"

ChessFrame::ChessFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(640, 640)) {
    Centre();
	wxImage::AddHandler(new wxPNGHandler());
}