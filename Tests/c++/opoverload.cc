#include <iostream>
#include <cmath>

class Integer
{
public:
    Integer(int value) : value(value) { std::cout << value << std::endl; }
    Integer operator+(const Integer &r)
    {
        return value + r.value;
    }
    Integer operator-(const Integer &r)
    {
        return Integer(value - r.value);
    }
    Integer operator*(const Integer &r)
    {
        return Integer(value * r.value);
    }
    Integer operator/(const Integer &r)
    {
        return Integer(value / r.value);
    }
    bool operator==(const Integer &r)
    {
        if (value == r.value)
            return true;
        else
            return false;
    }
    Integer operator^(const Integer &r)
    {
        return Integer(int(pow(value, r.value)));
    }
private:
    int value;
};

int main(void)
{
    Integer a(2);
    Integer b = 1026;
    if ((a ^ 10) + 1 == b - 1)
    {
        std::cout << "equal" << std::endl;
    }
    return 0;
}
