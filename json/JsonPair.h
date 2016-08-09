#pragma once

#include <string>
#include <memory>
#include <JsonValue.h>

namespace Json
{
	struct Pair
	{
		std::string key;
		std::shared_ptr<Value> value;
	};
}
