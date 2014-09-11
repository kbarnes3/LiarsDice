#include "stdafx.h"
#include "Console.h"

WORD CONSOLE_COLORS = FOREGROUND_GREEN;
DWORD CONSOLE_MODE = ENABLE_WINDOW_INPUT;

Console::Console() :
    m_hStdIn(INVALID_HANDLE_VALUE),
    m_hStdOut(INVALID_HANDLE_VALUE),
    m_originalMode(0),
    m_originalColors(0)
{
    // Nothing to do
}

Console::~Console()
{
    ResetConsoleState();
}

HRESULT Console::RuntimeClassInitialize()
{
    CreateHandles();
    SetConsoleState();
    return S_OK;
}

void Console::RunConsole()
{
    ClearScreen();
    InputLoop();
}

void Console::CreateHandles()
{
    m_hStdIn = CreateFileW(L"CONIN$",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr);
    ChkIf(m_hStdIn == INVALID_HANDLE_VALUE);

    m_hStdOut = CreateFileW(L"CONOUT$",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr);
    ChkIf(m_hStdOut == INVALID_HANDLE_VALUE);
}

void Console::SetConsoleState()
{
    ChkIf(!GetConsoleMode(m_hStdIn, &m_originalMode));
    ChkIf(!SetConsoleMode(m_hStdIn, CONSOLE_MODE));

    CONSOLE_SCREEN_BUFFER_INFO csbi = {};

    ChkIf(!GetConsoleScreenBufferInfo(m_hStdOut, &csbi));
    m_originalColors = csbi.wAttributes;
    ChkIf(!SetConsoleTextAttribute(m_hStdOut, CONSOLE_COLORS));
}

void Console::InputLoop()
{
    for (;;)
    {
        INPUT_RECORD rgInputs[128] = {};
        DWORD inputsRead = 0;

        ChkIf(!ReadConsoleInput(
            m_hStdIn,
            rgInputs,
            ARRAYSIZE(rgInputs),
            &inputsRead));

        for (DWORD i = 0; i < inputsRead; i++)
        {
            const INPUT_RECORD* pInput = rgInputs + i;
            switch (pInput->EventType)
            {
                case KEY_EVENT:
                    return;
                    break;
                case WINDOW_BUFFER_SIZE_EVENT:
                    // Handle resized console windows
                    break;
                default:
                    break;
            }
        }
    }
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

void Console::ResetConsoleState()
{
    if (m_hStdIn != INVALID_HANDLE_VALUE)
    {
        ChkIf(!SetConsoleMode(m_hStdIn, m_originalMode));
        CloseHandle(m_hStdIn);
    }

    if (m_hStdOut != INVALID_HANDLE_VALUE)
    {
        ChkIf(!SetConsoleTextAttribute(m_hStdOut, m_originalColors));
        ClearScreen();
        CloseHandle(m_hStdOut);
    }
}
