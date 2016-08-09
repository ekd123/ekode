#include <exception>
#include <string>
#include "JsonValue.h"

namespace Json
{
	const char *Value::GetTypeString(ValueType type)
	{
		switch(type)
		{
		case VALUE_TYPE_ARRAY:
			return "array";
		case VALUE_TYPE_BOOL:
			return "bool";
		case VALUE_TYPE_NULL:
			return "null";
		case VALUE_TYPE_NUMBER:
			return "number";
		case VALUE_TYPE_OBJECT:
			return "object";
		case VALUE_TYPE_STRING:
			return "string";
		default:
			return "???";
		}
	}
	
	ValueType String::GetType() const
	{
		return VALUE_TYPE_STRING;
	}
	
	ValueType Number::GetType() const
	{
		return VALUE_TYPE_NUMBER;
	}
	
	ValueType Bool::GetType() const
	{
		return VALUE_TYPE_BOOL;
	}
	
	ValueType Null::GetType() const
	{
		return VALUE_TYPE_NULL;
	}
	
	ValueType Array::GetType() const
	{
		return VALUE_TYPE_ARRAY;
	}
}
