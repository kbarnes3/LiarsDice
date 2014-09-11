#include "stdafx.h"
#include "Frame.h"

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
    m_hStdIn = hStdIn;
    m_hStdOut = hStdOut;
    m_width = width;
    m_height = height;

    return S_OK;
}

void Frame::InitializeFrame()
{
    WCHAR letters[] = L"These are some things to output";
    CHAR_INFO chars[ARRAYSIZE(letters) - 1] = {};
    COORD buffer_size = {ARRAYSIZE(chars), 1};
    COORD origin = {0, 0};
    SMALL_RECT rect = {0, 0, ARRAYSIZE(chars), 0};

    for (size_t i = 0; i < ARRAYSIZE(chars); i++)
    {
        CHAR_INFO* pChar = chars + i;
        pChar->Char.UnicodeChar = letters[i];
        pChar->Attributes = 2;
    }

    ChkIf(!WriteConsoleOutput(m_hStdOut,
        chars,
        buffer_size,
        origin,
        &rect));
}
