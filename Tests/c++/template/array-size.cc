#include <iostream>
using namespace std;

template <typename T, std::size_t Num>
constexpr size_t arrlen(T (&arr)[Num])
{
    return Num;
}

int main()
{
    int a[100] = { 0 };
    char b[1000] = { 0 };
    cout << arrlen(a) << " " << arrlen(b) << endl;
    return 0;
}
