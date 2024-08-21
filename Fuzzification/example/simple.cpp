#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>

int main(int argc, char **argv)
{
    int x = 0;
    std::string y;
    std::cin >> x;
    std::cin >> y;

    int i = 0;
    int j = 0;

    if (x == 0)
    {
        i = 1;
    }
    else
    {
        i = 2;
    }

    if (!std::strcmp(y.c_str(), "pass")) {
        j = 3;
    } else {
        j = 4;
    }

    std::cout << i << " " << j << std::endl;
    return 0;
}
