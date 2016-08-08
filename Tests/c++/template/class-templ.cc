#include <iostream>

static int FooCounter = 0;
static int VecCounter = 0;

class Foo
{
public:
    Foo() : c(++FooCounter)
    {
        std::clog << "init Foo " << c << std::endl;
    }
    ~Foo()
    {
        std::clog << "fin Foo " << c << std::endl;
    }
private:
    int c;
};

template <class T>
class vector
{
public:
    vector(size_t N) : blob(new T[N]), c(++VecCounter)
    {
        std::clog << "init vec " << c << std::endl;
    }
    vector(size_t N, T val) : blob(new T[N]), c(++VecCounter)
    {
        std::clog << "init vec " << c << std::endl;
        for(size_t i = 0; i < N; i++)
        {
            blob[i] = val;
        }
    }
    ~vector()
    {
        std::clog << "fin vec " << c << std::endl;
        delete[] blob;
        // if you write
        // delete blob;
        // here, only one fin Foo will be printed
        // and munmap_chunk(): invalid pointer
        // will be raised
    }
    T &operator[](int index)
    {
        return blob[index];
    }
    const T &operator[](int index) const
    {
        return blob[index];
    }
    T *get_bare()
    {
        return blob;
    }
private:
    T *blob;
    int c;
};

int main(void)
{
    vector<char> str(6, 0);
    str[0] = 'h';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    str[5] = 0;
    std::cout << str.get_bare() << std::endl;
    
    vector<Foo> vf(10);
    return 0;
}
