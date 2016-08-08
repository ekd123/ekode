#include <iostream>
using namespace std;

template<typename T>
int compare(const T &v1, const T &v2)
{
    if (v1 > v2) return 1;
    else if (v1 < v2) return -1;
    else return 0;
}

template<typename T>
T *swap_and_return(T *l, T *r)
{
    T temp;
    temp = *l;
    *l = *r;
    *r = temp;
    return l;
}

int main(int argc, char *argv[])
{
    int a = 1, b = 2;
    cout << "1, 2 : " << compare(1, 2) << endl
        << "1, 2 : " << compare(2, 1) << endl
        << "1, 1 : " << compare(1, 1) << endl
        << "swap : " << *swap_and_return(&a, &b) << endl;
    return 0;
}
