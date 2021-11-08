#include "Base.h"

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 직렬화로 프리팹을 만드는건 먼 훗날의 이야기. 지금은 특별한 구조를 짜서 저장하고 불러오는 식으로 구현하자..")
Base::Base() {}
Base::~Base() {}

void Base::draw_detail_view()
{
	if (ImGui::CollapsingHeader("Base"))
	{
		if (ImGui::TreeNode("Delenode"))
		{
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Safe ptr"))
		{
			std::string name;
			
			ImGui::TreePop();
		}
	}
}
