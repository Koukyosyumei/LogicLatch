#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include "crc.cpp"

int main(int argc, char **argv)
{
    int x = 0;
    std::cin >> x;

    int i = 0;

    if (modifyInt(x) == modifyInt(0))
    {
        i = 1;
    }

    std::cout << i << std::endl;
    return 0;
}
