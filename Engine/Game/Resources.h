#pragma once

#include "Resource/Resource.h"

template <typename friend_class>
class Resources : public Resource
{
public:
	std::map<Name, std::unique_ptr<Resource>> Resource_map;
public:
	virtual ~Resources() {}
	virtual std::type_index get_class_type() { return typeid(Resources); }

private:
	// 뭘 잘못했길래 접근이 안되는지 모르겠네.
	void set_resource(Name& resource_name, Resource* cast_result)
	{
		cast_result->Registed_name = resource_name;
		Resource_map.insert(make_pair(resource_name, std::unique_ptr<Resource>(cast_result)));
	}
public:
	template <typename Resource_type>
	SafePtr<Resource_type> add_resource(Name Resource_name, std::unique_ptr<Resource_type>& resource)
	{
		auto it = Resource_map.find(Resource_name);
		Resource* cast_result = dynamic_cast<Resource*>(resource.release());
		if (cast_result != nullptr)
		{
			set_resource(Resource_name, cast_result);
			return cast_result->get_safe_ptr().cast<Resource_type>();
		}
		else
		{
			return SafePtr<Resource_type>();
		}
		// 같은 리소스를 두번 집어넣으면 오류가 나는가?
	}
	SafePtr<Resource> get_resource(Name Resource_name)
	{
		auto Iter = Resource_map.find(Resource_name);
		if (Iter != Resource_map.end())
			return Iter->second->get_safe_ptr().cast<Resource>();
		else
			return SafePtr<Resource>();
	}
public:
	template<typename Resource>
	void load_resource(friend_class* owner, SafePtr<Resource> resource)
	{
		resource->load_resource(owner);
	}
	template<typename Resource>
	void release_resource(friend_class* owner, SafePtr<Resource> resource)
	{
		resource->release_resource(owner);
	}

	// Interface_ImGui을(를) 통해 상속됨

	/// <summary>
	/// 0 : 리소스 리스트 출력
	/// </summary>
	virtual void draw_detail_view() override
	{
		for (auto iter = this->Resource_map.begin(); iter != this->Resource_map.end(); ++iter)
		{
			ImGui::Text(iter->second->get_name().get_string().c_str());
		}
	}

	// Resource을(를) 통해 상속됨
	virtual void load_resource(Engine* engine) override
	{
	}
	virtual void release_resource(Engine* engine) override
	{
	}
};