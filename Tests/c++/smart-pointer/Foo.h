#pragma once

class Foo
{
public:
	Foo() { cout << "constructor" << endl; }
	~Foo() { cout << "destructor" << endl; }
	void Say(const char *message = "") const
	{
		cout << "Hi " << message << endl;
	}
};
