#pragma once

//ImGui
#include "../ImGui/imconfig.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui_internal.h"
//#include "imstb_rectpack.h"
//#include "imstb_textedit.h"
//#include "imstb_truetype.h"

#include "../ImGui-filebrowser/imgui_filebrowser.h"

#include <string>
#include <DirectXTex.h>
using namespace DirectX;
#include <vector>
#include <map>

class Base;
namespace ImGui
{
	bool RightAngleButton(const char* ch, float padding = 15);
	void RightAngle(float padding);
	bool sizeable_check_box(bool* bool_ptr, const char* char_ptr, float offset = 2.0f, float fontsize = 8.0f);
	void set_font_size(float re_font_size, float* backup = nullptr);

	template <typename FUNC = void(*)()>
	void SimpleTree(std::string label, FUNC func)
	{
		if (ImGui::TreeNode(label.c_str()))
		{
			func();
			ImGui::TreePop();
		}
	}
}


namespace ImGui
{
	////여기서 컨텐츠 탐색할때 코드 작성해야 함.
	//static string text_field = "";
	//if (ImGui::BeginPopup("Test"))
	//{
	//	ImGui::Text_field(text_field, "Add componenet", 100);
	//	auto& resource_map = this_scene->this_engine->resources.Resource_map;
	//	for (auto iter = resource_map.begin(); iter != resource_map.end(); ++iter)
	//	{
	//		if (iter->second->get_name().get_string().find(text_field) != string::npos)
	//		{
	//			// 해당하는 스트링이 있는 경우에만.
	//			ImGui::Button(iter->second->get_name().get_string().c_str(), ImVec2(-1, 0));
	//		}
	//	}
	//	ImGui::EndPopup();
	//}
 
	//template <typename TYPE, typename FUNC = void(*)(void)>
	//void SafePtr_field(SafePtr<TYPE>& safe_ptr, std::string name, FUNC func)
	//{
	//	ImGui::Text(typeid(TYPE).name());
	//	ImGui::SameLine();

	//	//std::string ID = name + "##" + StringHelper::ptr_to_string(&safe_ptr);
	//	if (ImGui::RightAngleButton(name.c_str()))
	//	{
	//		func();
	//	}
	//}

	void wstring_field(std::wstring& wstr, std::string name, size_t buff_size);

	bool INT_field(int* ptr, std::string name);
	bool INT2_field(int* ptr, std::string name);
	bool INT3_field(int* ptr, std::string name);

	bool float_field(float* ptr, std::string name);
	bool XMFLOAT2_field(XMFLOAT2& float3, std::string name);
	bool XMFLOAT3_field(XMFLOAT3& float3, std::string name);
	bool XMVECTOR_field(XMVECTOR& vec, std::string name);

	void XMMATRIX_field(XMMATRIX& mat, std::string str, std::string key);
	void XMMATRIX_field(XMMATRIX& mat, std::string display);

	void Text_field(std::string& text, std::string name, size_t buff_size);

	template<typename TYPE, typename FUNC = void(*)(TYPE&)>
	void list_field(std::list<TYPE>& list, FUNC func)
	{
		for (auto iter = list.begin(); iter != list.end(); ++iter)
		{
			func(*iter);
		}
	}
	template<typename TYPE, typename FUNC = void(*)(size_t, TYPE&)>
	void vector_field(std::vector<TYPE>& vec, FUNC func)
	{
		for (size_t i = 0; i < vec.size(); ++i)
		{
			func(i, vec[i]);
		}
	}
	template<typename KEY, typename VAL, typename FUNC = void(*)(KEY&, VAL&)>
	void map_field(std::map<KEY, VAL>& map, FUNC func)
	{
		for (auto iter = map.begin(); iter != map.end(); ++iter)
		{
			func(iter->first, iter->second);
		}
	}
}