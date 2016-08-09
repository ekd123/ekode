#pragma once

#include <vector>
#include <memory>

namespace Json
{
	typedef enum  {
		VALUE_TYPE_STRING,
		VALUE_TYPE_NUMBER,
		VALUE_TYPE_ARRAY,
		VALUE_TYPE_OBJECT,
		VALUE_TYPE_BOOL,
		VALUE_TYPE_NULL,
		VALUE_TYPE_INVALID
	} ValueType;
	
	struct Value
	{
		virtual ValueType GetType() const = 0;
		static const char *GetTypeString(ValueType type);
	};
	
	struct String : Value
	{
		virtual ValueType GetType() const;
		std::string data;
	};
	
	struct Number : Value
	{
		virtual ValueType GetType() const;
		int data;
	};
	
	struct Null : Value
	{
		virtual ValueType GetType() const;
	};
	
	struct Bool : Value
	{
		virtual ValueType GetType() const;
		bool data;
	};
	
	struct Array : Value
	{
		virtual ValueType GetType() const;
		std::vector<std::shared_ptr<Value>> data;
	};
}
