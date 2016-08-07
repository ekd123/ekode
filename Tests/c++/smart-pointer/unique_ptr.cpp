#include <memory>
#include <iostream>
using namespace std;

#include "Foo.h"

void TakeBare(Foo *ptr)
{
	ptr->Say("TakeBare");
	delete ptr; // take care of the destruction
}

void TakeAuto(Foo *bare)
{
	unique_ptr<Foo> ptr(bare);
	ptr->Say("TakeAuto");
	// automatically destroyed
}

int main(void)
{
	// Manually release
	unique_ptr<Foo> p1(new (nothrow) Foo());
	TakeBare(p1.release());  // transfer the ownership
	// now you can't access p1
	cout << "main" << endl;
	
	// Automatically release
	unique_ptr<Foo> p2(new (nothrow) Foo());
	TakeAuto(p2.release()); // transfer the ownership
	cout << "main" << endl;
	return 0;
}
