#pragma once

class Console : public RuntimeClass<IInspectable>
{
public:
    Console();
    ~Console();

    HRESULT RuntimeClassInitialize();
    void RunConsole();

private:
    void CreateHandles();
    void SetConsoleState();
    void InputLoop();
    void ClearScreen(WORD attributes);
    void ResetConsoleState();

    HANDLE m_hStdIn;
    HANDLE m_hStdOut;
    SHORT m_screenWidth;
    SHORT m_screenHeight;
    SHORT m_bufferWidth;
    SHORT m_bufferHeight;
    DWORD m_originalMode;
    WORD m_originalColors;
};
