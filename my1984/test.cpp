#include <iostream>
#include <string.h>
#include <windows.h>


int main2()
{
    LARGE_INTEGER a;
    LARGE_INTEGER b;
    LARGE_INTEGER freq;

    QueryPerformanceFrequency(&freq);

    QueryPerformanceCounter(&a);

    Sleep(1000);

    QueryPerformanceCounter(&b);

    printf("%lld\n", b.QuadPart - a.QuadPart);
    printf("%lf\n", (b.QuadPart - a.QuadPart) / (float)freq.QuadPart);

    return 0;
}