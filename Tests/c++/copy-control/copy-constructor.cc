#include <iostream>
#include <cstring>
using namespace std;

#define LOG(s) cout << s << endl

const int N = 10;

class Foo
{
public:
	char *strbuf;
	// default constructor
	Foo() : strbuf(nullptr)
	{
		LOG("constructor");
		for(auto &i : simpleVals)
		{
			i = 0;
		}
	}
	Foo(const char *str) :
		strbuf(strdup(str))
	{
		LOG("constructor");
		for(auto &i : simpleVals)
		{
			i = 0;
		}
	}
	~Foo()
	{
		LOG("destructor");
		delete strbuf;
	}
	// copy constructor (const)
	// a synthesized copy constructor will copy every field into the new object
	// including pointers!
	Foo(const Foo &src)
	{
		LOG("copy constructor");
		// simply copy them...
		for(int i = 0; i < N; i++)
		{
			simpleVals[i] = src.simpleVals[i];
		}
		// This is why we need copy constructors!
		// This is so-called a "deep copy"
		strbuf = strdup(src.strbuf);
	}
	void reset(const char *s)
	{
		delete strbuf;
		strbuf = strdup(s);
	}
	void Say(void) const
	{
		LOG("Say");
		cout << strbuf << endl;
	}
private:
	int simpleVals[N];
};

void takeCopy(const Foo obj)
{
	obj.Say();
	// the copy finalizes
}

void takeRef(const Foo &obj)
{
	obj.Say();
}

int main(void)
{
	Foo foo("first");
	Foo bar = foo; // calls the copy constructor
	if (!strcmp(foo.strbuf, bar.strbuf))
	{
		cout << "yes, they are the same strings" << endl;
	}
	cout << (void *)foo.strbuf << endl << (void *)bar.strbuf << endl;
	bar.reset("second");
	
	takeCopy(foo); // copy
	takeRef(bar); // no copy
	return 0;
}
