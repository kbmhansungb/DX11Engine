#include "Base.h"

Base::Base() {}
Base::~Base() {}

void Base::draw_detail_view()
{
	if (ImGui::CollapsingHeader("Base"))
	{
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 미완성 코드")
		// 이 인스턴스의 메서드를 참조하는 델리노드를 보여주어야 함.
		// 하지만 델리게이트는 이름을 가지고 있지 않아 표현할 방법이 없음.
		//if (ImGui::TreeNode("Delenode"))
		//{
		//	ImGui::TreePop();
		//}

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 미완성 코드")
		// 이 인스턴스의 세이프 피티알을 참조하는 변수를 보여주어야 함.
		// 하지만 세이프 참조는 이름을 가지고 있지 않아 표현할 방법이 없음.
		//if (ImGui::TreeNode("Safe ptr"))
		//{
		//	ImGui::TreePop();
		//}
	}
}
