#include "JsonObject.h"
#include "JsonValue.h"

using namespace std;

namespace Json
{
	void Object::AddMember(Pair member)
	{
		members.push_back(member);
	}
	
	ValueType Object::GetType() const
	{
		return VALUE_TYPE_OBJECT;
	}
	
	const vector<Pair> Object::GetMembers() const
	{
		return members;
	}
}
