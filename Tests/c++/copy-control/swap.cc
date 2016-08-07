#include <iostream>
#include <cstring>

class CString
{
public:
	CString() = delete;
	CString(const char *str) : buffer(strdup(str)) { }
	~CString() { delete buffer; }
	CString(const CString &src)
	{
		delete buffer;
		std::cout << "copy!" << std::endl;
		buffer = strdup(src.buffer);
	}
	char *buffer;
};

//namespace std {
	void swap(CString &lval, CString &rval)
	{
		// this is high-performance
		// because it doesn't copy the buffer itself
		char *temp;
		temp = lval.buffer;
		lval.buffer = rval.buffer;
		rval.buffer = temp;
	}
//}

int main(void)
{
	// the standard swap
	//     temp = a;
	//     a = b;
	//     b = temp;
	int a = 42, b = 0;
	std::swap(a, b);
	std::cout << 
		"a: " << a << std::endl <<
		"b: " << b << std::endl;
	
	CString str1("first");
	CString str2("second");
	//
	// customized swap
	//
	/*std::*/swap(str1, str2);
	std::cout << str1.buffer << " " << str2.buffer << std::endl;
	//
	// standard swap
	//
	// this not only causes performance problems
	// but also crashes here
	//std::swap(str1, str2);
	//std::cout << str1.buffer << " " << str2.buffer << std::endl;
	return 0;
}

