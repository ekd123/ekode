#include <iostream>
using namespace std;

class Foo
{
public:
	static int show(const char *a)
	{
		cout << a << endl;
		return 42;
	}
};

int plus_one(int a)
{
	return a + 1;
}

int main(void)
{
	int a = Foo::show("getting 42");
	int *pa = &a;
	cout << 
		"plus_one: " << sizeof(&plus_one) << endl <<
		"plus_two: " << sizeof(&Foo::show) << endl <<
		"pa: " << sizeof(pa) << endl;
	return 0;
}
