#include <iostream>

int add(int a, int b, int)
{
    return a + b;
}

int main(int argc, char *argv[])
{
    std::cout << add(1, 2, 3) << std::endl;
    return 0;
}
