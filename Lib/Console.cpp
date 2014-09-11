#include "stdafx.h"
#include "Console.h"

WORD CONSOLE_COLORS = FOREGROUND_GREEN;
DWORD CONSOLE_MODE = ENABLE_WINDOW_INPUT;

Console::Console() :
    m_hStdIn(INVALID_HANDLE_VALUE),
    m_hStdOut(INVALID_HANDLE_VALUE),
    m_width(0),
    m_height(0),
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
    ClearScreen(CONSOLE_COLORS);
    SampleDisplay();
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
    m_width = csbi.dwSize.X;
    m_height = csbi.dwSize.Y;
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

void Console::ClearScreen(WORD attributes)
{
    DWORD consoleSize = m_width * m_height;
    COORD origin = {0, 0};
    DWORD charsWritten = 0;

    ChkIf(!FillConsoleOutputCharacterW(m_hStdOut,
        L' ',
        consoleSize,
        origin,
        &charsWritten));

    ChkIf(!FillConsoleOutputAttribute(m_hStdOut,
        attributes,
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
        ClearScreen(m_originalColors);
        CloseHandle(m_hStdOut);
    }
}

void Console::SampleDisplay()
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
        pChar->Attributes = CONSOLE_COLORS;
    }

    ChkIf(!WriteConsoleOutput(m_hStdOut,
        chars,
        buffer_size,
        origin,
        &rect));
}
