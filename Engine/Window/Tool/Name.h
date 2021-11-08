#pragma once
#include <string>
#include <map>

namespace NAME
{
	class Name;
	class NameContainer;

	class Name
	{
		std::pair<std::string, unsigned int> Pair;
	public:
		Name()
		{
			Pair = std::make_pair("", 0);
		}
		Name(NameContainer* Name_container, std::string name);
		void operator=(const Name& rhs)
		{
			Pair = rhs.Pair;
		}
		bool operator==(const Name& rhs) const
		{
			return Pair.second == rhs.Pair.second;
		}
		bool operator<(const Name& _Right) const {
			return this->Pair.second < _Right.Pair.second;
		}
		std::string get_string()
		{
			return Pair.first;
		}
		unsigned int get_num()
		{
			return Pair.second;
		}
	};

	class NameContainer
	{
	public:
		std::map<std::string, unsigned int> map;
	};
}
typedef NAME::Name Name;
typedef NAME::NameContainer NameContainer;

//std::map<NAME, DATA> data_map;
//{
//	Name name = Name(&name_container, "Test name");
//	data_map[name] = DATA();
//}