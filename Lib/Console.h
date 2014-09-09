#pragma once

class Console : public RuntimeClass<IInspectable>
{
public:
    Console();
    ~Console();

    HRESULT RuntimeClassInitialize();

private:
    void ResetColors();

    HANDLE m_hStdOut;
    WORD m_originalColors;
};
