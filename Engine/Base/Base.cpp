#include "Base.h"

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: ����ȭ�� �������� ����°� �� �ʳ��� �̾߱�. ������ Ư���� ������ ¥�� �����ϰ� �ҷ����� ������ ��������..")
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
