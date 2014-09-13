#include "stdafx.h"
#include "Console.h"
#include "Frame.h"

WORD CONSOLE_COLORS = FOREGROUND_GREEN;
DWORD CONSOLE_MODE = ENABLE_WINDOW_INPUT;

Console::Console() :
    m_hStdIn(INVALID_HANDLE_VALUE),
    m_hStdOut(INVALID_HANDLE_VALUE),
    m_screenWidth(0),
    m_screenHeight(0),
    m_bufferWidth(0),
    m_bufferHeight(0),
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
    
    ComPtr<Frame> spFrame;
    Chk(MakeAndInitialize<Frame>(&spFrame, m_hStdIn, m_hStdOut, m_screenWidth, m_screenHeight));
    spFrame->InitializeFrame();

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
    ChkIf(m_hStdIn != INVALID_HANDLE_VALUE);

    m_hStdOut = CreateFileW(L"CONOUT$",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr);
    ChkIf(m_hStdOut != INVALID_HANDLE_VALUE);
}

void Console::SetConsoleState()
{
    ChkIf(GetConsoleMode(m_hStdIn, &m_originalMode));
    ChkIf(SetConsoleMode(m_hStdIn, CONSOLE_MODE));

    CONSOLE_SCREEN_BUFFER_INFO csbi = {};

    ChkIf(GetConsoleScreenBufferInfo(m_hStdOut, &csbi));
    m_screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    m_screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    m_bufferWidth = csbi.dwSize.X;
    m_bufferHeight = csbi.dwSize.Y;
    m_originalColors = csbi.wAttributes;
    ChkIf(SetConsoleTextAttribute(m_hStdOut, CONSOLE_COLORS));
}

void Console::InputLoop()
{
    for (;;)
    {
        INPUT_RECORD rgInputs[128] = {};
        DWORD inputsRead = 0;

        ChkIf(ReadConsoleInput(
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
    DWORD bufferSize = m_bufferWidth * m_bufferHeight;
    COORD origin = {0, 0};
    DWORD charsWritten = 0;

    ChkIf(FillConsoleOutputCharacterW(m_hStdOut,
        L' ',
        bufferSize,
        origin,
        &charsWritten));

    ChkIf(FillConsoleOutputAttribute(m_hStdOut,
        attributes,
        bufferSize,
        origin,
        &charsWritten));

    ChkIf(SetConsoleCursorPosition(m_hStdOut, origin));

    return;
}

void Console::ResetConsoleState()
{
    if (m_hStdIn != INVALID_HANDLE_VALUE)
    {
        ChkIf(SetConsoleMode(m_hStdIn, m_originalMode));
        CloseHandle(m_hStdIn);
    }

    if (m_hStdOut != INVALID_HANDLE_VALUE)
    {
        ChkIf(SetConsoleTextAttribute(m_hStdOut, m_originalColors));
        ClearScreen(m_originalColors);
        CloseHandle(m_hStdOut);
    }
}
