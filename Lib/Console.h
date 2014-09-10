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
    void ClearScreen();
    void ResetColors();

    HANDLE m_hStdOut;
    WORD m_originalColors;
};
