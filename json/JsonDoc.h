#pragma once

#include <string>
#include <JsonObject.h>

namespace Json
{
	class Doc
	{
	public:
		Doc() = default;
		~Doc() = default;
		void LoadFromText(const char *doctxt);
		const Object &GetRoot(void);
	private:
		void DoParse(const char *doctxt);
		Object root;
	};
}
