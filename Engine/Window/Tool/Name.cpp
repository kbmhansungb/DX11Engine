#include "Name.h"

Name::Name(NameContainer* Name_container, std::string Name)
{
	auto It = Name_container->map.find(Name);
	if (It != Name_container->map.end())
	{
		this->Pair.first = It->first;
		this->Pair.second = It->second;
	}
	else
	{
		this->Pair = std::make_pair(Name, Name_container->map.size() + 1);
		Name_container->map.insert(this->Pair);
	}
}
