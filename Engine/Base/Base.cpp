#include "Base.h"

Base::Base() {}
Base::~Base() {}

void Base::draw_detail_view()
{
	if (ImGui::CollapsingHeader("Base"))
	{
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: �̿ϼ� �ڵ�")
		// �� �ν��Ͻ��� �޼��带 �����ϴ� ������带 �����־�� ��.
		// ������ ��������Ʈ�� �̸��� ������ ���� �ʾ� ǥ���� ����� ����.
		//if (ImGui::TreeNode("Delenode"))
		//{
		//	ImGui::TreePop();
		//}

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: �̿ϼ� �ڵ�")
		// �� �ν��Ͻ��� ������ ��Ƽ���� �����ϴ� ������ �����־�� ��.
		// ������ ������ ������ �̸��� ������ ���� �ʾ� ǥ���� ����� ����.
		//if (ImGui::TreeNode("Safe ptr"))
		//{
		//	ImGui::TreePop();
		//}
	}
}
