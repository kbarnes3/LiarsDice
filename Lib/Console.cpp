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
    CreateHandles();
    return S_OK;
}

void Console::RunConsole()
{
    ClearScreen();
}

void Console::CreateHandles()
{
    m_hStdOut = CreateFileW(L"CONOUT$",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr);
    ChkIf(m_hStdOut == INVALID_HANDLE_VALUE);

    CONSOLE_SCREEN_BUFFER_INFO csbi = {};

    ChkIf(!GetConsoleScreenBufferInfo(m_hStdOut, &csbi));
    m_originalColors = csbi.wAttributes;

}

void Console::ClearScreen()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi = {};

    ChkIf(!GetConsoleScreenBufferInfo(m_hStdOut, &csbi));

    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD origin = {0, 0};
    DWORD charsWritten = 0;

    ChkIf(!FillConsoleOutputCharacterW(m_hStdOut,
        L' ',
        consoleSize,
        origin,
        &charsWritten));

    ChkIf(!FillConsoleOutputAttribute(m_hStdOut,
        csbi.wAttributes,
        consoleSize,
        origin,
        &charsWritten));

    ChkIf(!SetConsoleCursorPosition(m_hStdOut, origin));

    return;
}

void Console::ResetColors()
{
    if (m_hStdOut != INVALID_HANDLE_VALUE)
    {
        ChkIf(!SetConsoleTextAttribute(m_hStdOut, m_originalColors));
    }
}
