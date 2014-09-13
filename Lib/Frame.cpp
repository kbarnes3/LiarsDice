#include "stdafx.h"
#include "Frame.h"

WORD BORDER_COLOR = FOREGROUND_GREEN | FOREGROUND_RED;

Frame::Frame() :
    m_hStdIn(INVALID_HANDLE_VALUE),
    m_hStdOut(INVALID_HANDLE_VALUE),
    m_width(0),
    m_height(0)
{
    // Nothing to do
}

Frame::~Frame()
{
    // Nothing to do
}

HRESULT Frame::RuntimeClassInitialize(HANDLE hStdIn, HANDLE hStdOut, WORD width, WORD height)
{
    ChkIf(hStdIn != INVALID_HANDLE_VALUE);
    ChkIf(hStdOut != INVALID_HANDLE_VALUE);
    ChkIf(width >= 1);
    ChkIf(height >= 1);

    m_hStdIn = hStdIn;
    m_hStdOut = hStdOut;
    m_width = width;
    m_height = height;

    return S_OK;
}

void Frame::InitializeFrame()
{
    DisplayBorders();
}

void Frame::DisplayBorders()
{
    DisplayVerticalBorders();
    DisplayHorizontalBorders();
}

void Frame::DisplayVerticalBorders()
{
    CHAR_INFO* chars = new CHAR_INFO[m_height];
    ChkIf(chars);

    for (size_t i = 0; i < m_height; i++)
    {
        CHAR_INFO* pChar = chars + i;
        pChar->Char.UnicodeChar = L'|';
        pChar->Attributes = BORDER_COLOR;
    }

    COORD buffer_size = {1, m_height};
    COORD origin = {0, 0};

    SMALL_RECT left_rect = {0, 0, 0, m_height - 1};
    ChkIf(WriteConsoleOutputW(m_hStdOut,
        chars,
        buffer_size,
        origin,
        &left_rect));

    SMALL_RECT right_rect = {m_width - 1, 0, m_width - 1, m_height - 1};
    ChkIf(WriteConsoleOutputW(m_hStdOut,
        chars,
        buffer_size,
        origin,
        &right_rect));
}

void Frame::DisplayHorizontalBorders()
{
    CHAR_INFO* chars = new CHAR_INFO[m_width];
    ChkIf(chars);

    for (size_t i = 0; i < m_width; i++)
    {
        CHAR_INFO* pChar = chars + i;
        pChar->Char.UnicodeChar = L'=';
        pChar->Attributes = BORDER_COLOR;
    }

    COORD buffer_size = {m_width, 1};
    COORD origin = {0, 0};

    SMALL_RECT top_rect = {0, 0, m_width - 1, 0};
    ChkIf(WriteConsoleOutputW(m_hStdOut,
        chars,
        buffer_size,
        origin,
        &top_rect));

    SMALL_RECT bottom_rect = {0, m_height - 1, m_width - 1, m_height - 1};
    ChkIf(WriteConsoleOutputW(m_hStdOut,
        chars,
        buffer_size,
        origin,
        &bottom_rect));
}
