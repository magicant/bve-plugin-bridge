// exemain.cpp : executable entry point

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define ATS_IMPORTS

#include <iostream>
#include "../encoder.h"

int main(void)
{
    ATS_VEHICLESTATE v;
    std::cin >> v;
    std::cout << v;
    return 0;
}
