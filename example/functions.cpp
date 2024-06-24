#include <stdio.h>

int f1(int x) {
    int y = x * 2;
    return y;
}

int f2(int x) {
    int y = f1(x);
    int z = y + 1;
    return z;
}

int f3(int x) {
    if (x == 0) {
        return f1(x);
    } else {
        return f2(x);
    }
}

int main(int argc, char **argv)
{
    int i;
    if (argc == 0)
    {
        i = 1;
    }
    else
    {
        i = 2;
    }

    int j = f1(i);
    int k = f2(i);
    int l = f3(i);

    return j + k + l;
}
