// LiarsDice.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Console.h"

int main(void)
{
    ComPtr<Console> spConsole;
    Chk(MakeAndInitialize<Console>(&spConsole));
    spConsole->RunConsole();

    return 0;
}
