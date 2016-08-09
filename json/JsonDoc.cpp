#include <string>
#include <exception>
#include <ctype.h>
#include <memory>
#include "JsonDoc.h"
#include "JsonObject.h"
#include "JsonPair.h"

#ifndef NDEBUG
#define LOG printf
#else
#define LOG(...)
#endif

namespace Json
{
	using namespace std;
	
	const Object &Doc::GetRoot(void)
	{
		return root;
	}
	
	void Doc::LoadFromText(const char *doctxt)
	{
		DoParse(doctxt);
	}
	
	const char *ParseString(const char *t, string &result)
	{
		t++; // skip "
		while (true)
		{
			if (!*t)
				throw runtime_error("string not closed");
			else if (*t == '"')
			{
				LOG("%s\n", result.c_str());
				return t + 1;
			}
			else if (*t == '\\')
			{
				switch(t[1])
				{
				// edge case {'\', 0}
				case 0:
					throw runtime_error("string not closed");
					break;
				case '"':
					result += '"';
					break;
				case '\\':
					result += '\\';
					break;
				case '/':
					result += '/';
					break;
				case 'b':
					result += '\b';
					break;
				case 'f':
					result += '\f';
					break;
				case 'n':
					result += '\n';
					break;
				case 'r':
					result += '\r';
					break;
				case 't':
					result += '\t';
					break;
				case 'u':
					throw runtime_error("not implemented");
				default:
					result += t[1];
				}
				t++;
				continue;
			}
			else 
			{
				result += *t;
			}
			t++;
		}
		return nullptr; // unreachable
	}
	
	static const char *ParseNull(const char *t)
	{
		if (t[0] == 'n' && t[1] == 'u' && t[2] == 'l' && t[3] == 'l')
		{
			return t + 4;
		}
		throw runtime_error("illegal symbol in null");
	}
	
	static const char *ParseBool(const char *t, bool &result)
	{
		if (t[0] == 't' && t[1] == 'r' && t[2] == 'u' && t[3] == 'e')
		{
			result = true;
			return t + 4;
		}
		else if (t[0] == 'f' && t[1] == 'a' && t[2] == 'l' && t[3] == 's' && t[4] == 'e')
		{
			result = false;
			return t + 5;
		}
		else
		{
			throw runtime_error("illegal symbol in bool");
		}
	}
	
	static const char *ParseNumber(const char *t, Number &num)
	{
		num.data = 0;
		bool sig = 1;
		if (*t == '-')
		{
			sig = -1;
			t++; // skip -
		}
		while (*t && isdigit(*t))
		{
			num.data *= 10;
			num.data += *t - '0';
			t++;
		}
		num.data = sig * num.data;
		return t;
	}
	
	static const char *ParseObject(const char *t, Object &obj);
	static const char *ParseArray(const char *t, Array &array)
	{
		t++; // skip [
		// expects ], number, {
		// if it's an empty array
		while (*t && isspace(*t))
			t++;
		if (*t == ']')
			return t + 1;
		// if it's not an empty array
		while (true)
		{
			while (*t && isspace(*t))
				t++;
			switch(*t)
			{
			case 0:
				throw runtime_error("array not closed");
				break;
			case ']':
				return t + 1; // skip ]
				break;
			case '[':
				{
					auto ptr = make_shared<Array>();
					t = ParseArray(t, *ptr);
					array.data.push_back(ptr);
				}
				break;
			case '{':
				{
					auto ptr = make_shared<Object>();
					t = ParseObject(t, *ptr);
					array.data.push_back(ptr);
				}
				break;
			case '"':
				{
					auto s = make_shared<String>();
					t = ParseString(t, s->data);
					array.data.push_back(s);
				}
				break;
			case '-':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				{
					auto ptr = make_shared<Number>();
					t = ParseNumber(t, *ptr);
					array.data.push_back(ptr);
				}
				break;
			case 'n':
				{
					t = ParseNull(t);
					array.data.push_back(make_shared<Null>());
				}
				break;
			case 't':
			case 'f':
				{
					auto val = make_shared<Bool>();
					t = ParseBool(t, val->data);
					array.data.push_back(val);
				}
				break;
			case ',':
				t++; // skip ,
				continue;
			default:
				throw runtime_error("illegal character in array");
			}
		}
		return nullptr; // never reach here
	}
	
	static const char *ParseObject(const char *t, Object &obj)
	{
		t++; // skip {
		// if it's an empty doc
		while (*t && isspace(*t))
			t++;
		if (*t == '}')
			return t + 1;
		// if it's not an empty doc
		while(true)
		{
			Pair pair;
			// get name
			while (*t && isspace(*t))
				t++;
			if (*t != '"')
				throw runtime_error("pair.car not string");
			string key;
			t = ParseString(t, key); // get the name of a pair
			pair.key = key;
			
			// find :
			while (*t && isspace(*t))
				t++;
			if (*t != ':')
				throw runtime_error("there must be a value");
			t++; // skip colon
			
			// get value
			while (*t && isspace(*t))
				t++;
			switch(*t)
			{
			case 0:
				throw runtime_error("there must be a value");
			case '"':
				{
					auto val = make_shared<String>();
					t = ParseString(t, val->data);
					pair.value = val;
				}
				break;
			case '[':
				{
					auto val = make_shared<Array>();
					t = ParseArray(t, *val);
					pair.value = val;
				}
				break;
			case '{':
				{
					auto val = make_shared<Object>();
					t = ParseObject(t, *val);
					pair.value = val;
				}
				break;
			case 't':
			case 'f':
				{
					auto val = make_shared<Bool>();
					t = ParseBool(t, val->data);
					pair.value = val;
				}
				break;
			case '-':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				{
					auto ptr = make_shared<Number>();
					t = ParseNumber(t, *ptr);
					pair.value = ptr;
				}
				break;
			case 'n':
				{
					t = ParseNull(t);
					pair.value = make_shared<Null>();
				}
				break;
			default:
				throw runtime_error("illegal character in object");
			}
			
			// add pair to object
			obj.AddMember(pair);
			
			// expect , and }
			while (*t && isspace(*t))
				t++;
			switch (*t)
			{
			case ',':
				t++; // skip ,
				continue;
			case '}':
				goto quit_loop;
			}
		}
		quit_loop:
		return t + 1; // skip }
	}
	
	void Doc::DoParse(const char *doctxt)
	{
		while (*doctxt && isspace(*doctxt))
			doctxt++;
		if (*doctxt != '{')
			throw runtime_error("there must be a root");
		ParseObject(doctxt, root);
	}
}
