#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

class Board;

class ChessPanel : public wxPanel {
    public:
    ChessPanel(wxFrame *parent, Board *board);

    private:
    void OnPaint(wxPaintEvent& event);
    void OnLeftMouseDown(wxMouseEvent& event);

    void clearBuffer(wxGraphicsContext *gc);
    void handleScreenSelection(const wxPoint& point);

    void drawBoard(wxGraphicsContext* gc);
    void drawPieces(wxGraphicsContext* gc);
    void drawSelectionScreen(wxGraphicsContext* gc);

    Board *board;
    bool enemySelected;
};