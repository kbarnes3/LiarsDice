#include "stdafx.h"
#include "Test.h"

void ClearScreen(HANDLE hConsole)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi = {};

    ChkIf(!GetConsoleScreenBufferInfo(hConsole, &csbi));

    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD origin = {0, 0};
    DWORD charsWritten = 0;

    ChkIf(!FillConsoleOutputCharacterW(hConsole,
        L' ',
        consoleSize,
        origin,
        &charsWritten));

    ChkIf(!FillConsoleOutputAttribute(hConsole,
        csbi.wAttributes,
        consoleSize,
        origin,
        &charsWritten));

    ChkIf(!SetConsoleCursorPosition(hConsole, origin));

    return;
}
