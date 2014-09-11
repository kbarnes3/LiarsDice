#pragma once

class Frame : public RuntimeClass<IInspectable>
{
public:
    Frame();
    ~Frame();
    HRESULT RuntimeClassInitialize(HANDLE hStdIn, HANDLE hStdOut, WORD width, WORD height);

    void InitializeFrame();

private:
    HANDLE m_hStdIn;
    HANDLE m_hStdOut;
    WORD m_width;
    WORD m_height;
};
