#include <iostream>
#include <memory>
using namespace std;

#include "Foo.h"

void Take(const Foo *bare)
{
	bare->Say("Take");
}

int main(void)
{
	// this is how you normally use shared_ptr.
	// on every copy, the refcount is incr'd and the object will be destroyed
	// when nobody refers to it
	auto p = make_shared<Foo>();
	p->Say("main");
	Take(p.get());  // get the bare pointer but (FIXME) it's dangerous!
					// do this only when you understand what you are doing
	
	// weak_ptr does NOT increment the reference count
	// weak_ptr is used to break the reference cycle
	weak_ptr<Foo> q(p);
	if(shared_ptr<Foo> ptr = q.lock())
	{
		ptr->Say("weak_pointer");
	}
	else
	{
		cout << "Oops! The object has been destructed!" << endl;
	}
	return 0;
}
