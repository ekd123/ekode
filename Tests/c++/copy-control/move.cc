#include <iostream>
#include <utility>
#include <cstring>

class CString
{
public:
	CString(const char *s) : buffer(strdup(s))
	{
		std::cout << "constructor" << std::endl;
	}
	CString(const CString &src) : buffer(strdup(src.buffer))
	{
		std::cout << "copy" << std::endl;
	}
	CString(CString &&src) noexcept : buffer(src.buffer)
	{
		std::cout << "move constructor" << std::endl;
		src.buffer = nullptr; // make src safe to destroy
	}
	CString &operator=(CString &&rval) noexcept
	{
		std::cout << "move set" << std::endl;
		if(this != &rval)
		{
			delete buffer;
			buffer = rval.buffer;
			rval.buffer = nullptr;
		}
		return *this;
	}
	~CString()
	{
		delete buffer;
		std::cout << "destructor" << std::endl;
	}
	char *buffer;
};

int main(void)
{
	///
	/// r-value reference
	///
	int &&rr = 42;
	
	///
	/// get an rval reference from lval
	/// and lval is not accessible now
	///
	int lval = 42;
	int &&rr2 = std::move(lval);
	// std::cout << lval << std::endl; // considered bad though compilable
	
	///
	/// std::move
	///
	CString src("source");
	CString dest = std::move(src);
	std::cout << dest.buffer << std::endl;
	return 0;
}
