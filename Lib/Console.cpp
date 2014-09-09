#include "stdafx.h"
#include "Console.h"

Console::Console() :
    m_hStdOut(INVALID_HANDLE_VALUE),
    m_originalColors(0)
{
    // Nothing to do
}

Console::~Console()
{
    ResetColors();
}

HRESULT Console::RuntimeClassInitialize()
{
    m_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    ChkIf(m_hStdOut == INVALID_HANDLE_VALUE);

    CONSOLE_SCREEN_BUFFER_INFO csbi = {};

    ChkIf(!GetConsoleScreenBufferInfo(m_hStdOut, &csbi));
    m_originalColors = csbi.wAttributes;

    return S_OK;
}

void Console::ResetColors()
{
    SetConsoleTextAttribute(m_hStdOut, m_originalColors);
}
