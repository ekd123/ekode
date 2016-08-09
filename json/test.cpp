#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <fstream>
#include "json.h"
using namespace std;

const char *doc1 = "{"
"  \"key\": \"value\","
"  \"inside\": { \"embedded\": \"object\", \"another\"\t:\t\"object\"},"
"  \"int\": -100,"
"  \"支持\": [\"UTF\", 8],"
"  \"array\": [1, 2, 3, 4, 5, \"hel\rlo\"]"
"}";

void print_object(const Json::Object &obj, int level = 0);
void print_data(shared_ptr<Json::Value> value, int level = 0);
void print_array(const Json::Array &array, int level = 0);

void print_array(const Json::Array &array, int level)
{
	vector<shared_ptr<Json::Value>> elements = array.data;
	cout << endl;
	string levelstr(level, ' ');
	for (auto i : elements)
	{
		cout << levelstr << " - ";
		print_data(i, level + 2);
		cout << endl;
	}
}

void print_data(shared_ptr<Json::Value> value, int level)
{
	switch(value->GetType())
	{
	case Json::VALUE_TYPE_NUMBER:
		cout << ((Json::Number *)value.get())->data;
		break;
	case Json::VALUE_TYPE_STRING:
		cout << ((Json::String *)value.get())->data;
		break;
	case Json::VALUE_TYPE_ARRAY:
		print_array(*((Json::Array *)value.get()), level + 2);
		break;
	case Json::VALUE_TYPE_OBJECT:
		print_object(*((Json::Object *)value.get()), level + 2);
		break;
	}
}

void print_object(const Json::Object &obj, int level)
{
	const vector<Json::Pair> members = obj.GetMembers();
	string levelstr(level, ' ');
	for (const auto &i : members)
	{
		cout << levelstr << "<key: " << i.key << "> <" << 
			Json::Value::GetTypeString(i.value->GetType()) <<
			": ";
		print_data(i.value, level);
		cout << '>' << endl;
	}
}

int main(int argc, char *argv[])
{
	Json::Doc doc;
	doc.LoadFromText(doc1);
    Json::Object root = doc.GetRoot();
	print_object(root);
	
	// Code used in the benchmark
	//ifstream fin("test.json");
	//ostringstream tmp;
	//tmp << fin.rdbuf();
	//Json::Doc fdoc;
	//fdoc.LoadFromText(tmp.str().c_str());
	//print_object(fdoc.GetRoot());
	return 0;
}
