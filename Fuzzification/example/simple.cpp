#include <stdio.h>
#include <iostream>

int main(int argc, char **argv)
{
    int x = 0;
    std::cin >> x;

    int i = 0;
    if (x == 0)
    {
        i = 1;
    }
    else
    {
        i = 2;
    }
    std::cout << i << std::endl;
    return 0;
}
