#pragma once

#include "_Include.h"
#include "Scene.h"
#include "Resources.h"
#include "ResourceImporter.h"
#include "Engine_context.h"

class Engine : public Base, public EngineContext
{
public:
	Engine(HINSTANCE hIns);
	virtual ~Engine() {};
	virtual std::type_index get_class_type() { return typeid(Engine); }

public: // �⺻ ����

private:	HINSTANCE _hInstance;
public:		HINSTANCE get_hinstance() { return _hInstance; }
			__declspec(property (get = get_hinstance)) HINSTANCE hInstance;

private:	bool _engine_activation = true;
public:		void put_engine_activation(bool new_activation) { _engine_activation = new_activation; }
			__declspec(property (put = put_engine_activation)) bool engine_activation;
	
	EngineContext* engine_context;
	Scene root_scene;

	// Ȯ��.
	NameContainer name_container;
	Timer<Engine> timer;
	Resources<Engine> resources;

public:
	void LoopCall();
};

namespace ImGui
{
	//// ���� ���� ����°� ���ʿ��� ����ΰ�.
	//enum class list_field_default_state
	//{
	//	DEFAULT, DELETED, IS_CHANGED
	//};
	template<typename VAL, typename FUNC = void(*)(VAL&)>
	void list_field_default(
		std::list<VAL>& list,
		std::string name,
		FUNC func
	)
	{
		std::string this_context
			= StringHelper::ptr_to_string(&list);
		std::string tree_name
			= name + "##"
			+ this_context;

		if (ImGui::TreeNode(tree_name.c_str()))
		{
			std::string add_button_name
				= "Add##"
				+ this_context;
			if (ImGui::RightAngleButton(add_button_name.c_str()))
			{
				list.resize(list.size() + 1);
			}

			auto delete_target = list.end();
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				std::string delete_button
					= "X##"
					+ StringHelper::ptr_to_string(&*iter);
				if (ImGui::Button(delete_button.c_str()))
				{
					delete_target = iter;
				}
				ImGui::SameLine();
				func(*iter);
			}

			if (delete_target != list.end())
			{
				list.erase(delete_target);

			}
			ImGui::TreePop();
		}
	}

	template<typename KEY, typename VAL,
		typename KEY_FUNC = void(*)(const KEY& original, KEY& changed),
		typename VAL_FUNC = void(*)(const KEY&, VAL&)>
	void map_field_default(
		std::map<KEY, VAL>& map,
		KEY default_key,
		VAL default_val,
		KEY_FUNC key_func,
		VAL_FUNC val_func)
	{
		// �߰� �ʵ�.
		if (ImGui::RightAngleButton(
			("Add pair##" + StringHelper::ptr_to_string(&map)).c_str()
		))
		{
			if (map.size() == 0)
			{
				map[default_key] = default_val;
			}
			else
			{
				auto key = map.rbegin()->first;
				++key;
				map[key] = default_val;
			}
		}

		auto delete_iter = map.end();
		KEY key;
		bool move_pair = false;
		for (auto pair = map.begin(); pair != map.end(); ++pair)
		{
			if (key_func(pair->first, key))
			{
				move_pair = true;
				delete_iter = pair;
			}
			std::string button_name = "delete##" + StringHelper::ptr_to_string(&pair->second);
			if (ImGui::RightAngleButton(button_name.c_str()))
			{
				delete_iter = pair;
			}
			val_func(pair->first, pair->second);
		}
		if (move_pair)
		{
			map.insert(std::make_pair(key, delete_iter->second));
		}
		if (delete_iter != map.end())
		{
			map.erase(delete_iter);
		}
	}

	enum class base_field_state
	{
		IS_OPEN, IS_CHANGED, IS_CLOSED,
	};
	struct base_field_instance
	{
		base_field_state state = base_field_state::IS_CLOSED;
		char popup_search_buf[100];
	};
	// �� �˾��� ������ ���� ������ �ϳ��̱� ������ ������ ���.
	base_field_instance* get_base_field_instance();

	// ��������..
//#define make_enum_elum(TYPE, elum) TYPE::elum, #elum
//	template <typename TYPE>
//	void enum_field_add_elum(TYPE elum, std::string elum_str)
//	{
//
//	}
//	template <typename TYPE>
//	void enum_field(TYPE elum, std::string elum_str)
//	{
//		enum_field_add_elum(elum, elum_str);
//	}
//	template <typename TYPE, typename... ARG>
//	void enum_field(TYPE elum, std::string elum_str, ARG... arg)
//	{
//		enum_field_add_elum(elum, elum_str);
//
//		enum_filed(arg...);
//	}

	//void base_field_resource_start();
	Component* get_selected_component();
	void base_field_component_start(bool(*func)(Base* b));
	void base_field_game_object_start(GameObject*& result);
	void base_field_scene_start(Scene*& scene);
	/// <summary>
	/// 
	/// ���� ����ִ� ���� �������� ���̴� ���� ������
	/// ������ �� �ִ� ��ư���� ������.
	/// 
	/// </summary>
	/// <typeparam name="TYPE"></typeparam>
	/// <param name="type"></param>
	/// <param name="name"></param>
	template <typename TYPE>
	void base_field(SafePtr<TYPE>& type, std::string name = "")
	{
		// base field ���� ������Ʈ
		auto bfi = get_base_field_instance();
		bfi->state = base_field_state::IS_CLOSED;

		const auto& typeinfo = typeid(TYPE);
		TYPE default_type;
		auto resource		= dynamic_cast<Resource*>(&default_type);
		auto component		= dynamic_cast<Component*>(&default_type);
		auto game_object	= dynamic_cast<GameObject*>(&default_type);
		auto scene			= dynamic_cast<Scene*>(&default_type);

		SafePtr<Resource> res		= type.cast<Resource>();
		SafePtr<Component> com		= type.cast<Component>();
		SafePtr<GameObject> gam		= type.cast<GameObject>();
		SafePtr<Scene> sce			= type.cast<Scene>();

		// �ʵ� ����
		std::string this_context = StringHelper::ptr_to_string(&type);
		std::string label;
		if (name == "")
			label = typeinfo.name();
		else
			label = name;
		label 
			+= "##"
			+ this_context;
		std::string display_text;

		if (res.is_vaild())
		{
			display_text = res->get_name().get_string();
		}
		else if (com.is_vaild())
		{
			display_text 
				= com->owner->name
				+ " "
				+ typeid(*type.get()).name();
		}
		else if (gam.is_vaild())
		{
			display_text
				= gam->name;
		}
		else if (sce.is_vaild())
		{
			display_text
				= sce->name;
		}
		//display_text += "##" + this_context;

		// combo box�� ������.
		if (ImGui::BeginCombo(label.c_str(), display_text.c_str()))
		{
			bfi->state = base_field_state::IS_OPEN;

			std::string search_field
				= "##Search_index_"
				+ StringHelper::ptr_to_string(bfi->popup_search_buf);
			std::string search_name = bfi->popup_search_buf;
			std::string button_name
				= "Empty##"
				+ StringHelper::ptr_to_string(bfi->popup_search_buf);

			// ��ġ �ؽ�Ʈ �����
			ImGui::InputText(
				search_field.c_str(),
				bfi->popup_search_buf,
				100
			);
			// null�� ä��� ��ư
			if (ImGui::RightAngleButton(button_name.c_str()))
			{
				type = SafePtr<TYPE>();
			}

			// Ž�� �ʵ�
			bool is_selected = false;
			if (resource != nullptr)
			{
				auto shared = EDITOR::Shared::get_shared();
				for (auto& resource_pair : shared->engine->resources.Resource_map)
				{
					NAME::Name name = resource_pair.first;
					if (dynamic_cast<TYPE*>(resource_pair.second.get()) != nullptr &&
						name.get_string().find(search_name) != std::string::npos)
					{
						is_selected = false;
						ImGui::Selectable(name.get_string().c_str(), &is_selected);
						if (is_selected)
						{
							type = resource_pair.second.get()->get_safe_ptr().cast<TYPE>();
							bfi->state = base_field_state::IS_CHANGED;
						}
					}
				}
			}
			else if (component != nullptr)
			{
				base_field_component_start([](Base* b) 
					{
						return dynamic_cast<TYPE*>(b) != nullptr;
					});
				if (get_selected_component() != nullptr)
				{
					type = get_selected_component()->get_safe_ptr().cast<TYPE>();
					bfi->state = base_field_state::IS_CHANGED;
				}
			}
			else if (game_object != nullptr)
			{
				GameObject* gm = nullptr;
				base_field_game_object_start(gm);
				if (gm != nullptr)
				{
					type = gm->get_safe_ptr().cast<TYPE>();
					bfi->state = base_field_state::IS_CHANGED;
				}
			}
			else if (scene != nullptr)
			{
				Scene* sn = nullptr;
				base_field_scene_start(sn);
				if (sn != nullptr)
				{
					type = sn->get_safe_ptr().cast<TYPE>();
					bfi->state = base_field_state::IS_CHANGED;
				}
			}

			ImGui::EndCombo();
		}
		return;
	}
}