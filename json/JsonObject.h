#pragma once

#include <vector>
#include <string>
#include <JsonValue.h>
#include <JsonPair.h>

namespace Json
{
	class Object : public Value
	{
	public:
		Object() = default;
		~Object() = default;
		void AddMember(Pair member);
		const std::vector<Pair> GetMembers() const;
		virtual ValueType GetType(void) const;
	private:
		std::vector<Pair> members;
	};
}
