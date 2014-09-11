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
    void ClearScreen();
    void ResetConsoleState();

    HANDLE m_hStdIn;
    HANDLE m_hStdOut;
    DWORD m_originalMode;
    WORD m_originalColors;
};
