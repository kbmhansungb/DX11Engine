#pragma once

#include "../_Include.h"

/// <summary>
///  다형성을 보장하는 엔진의 기본 클래스, Base를 상속받아 구현해야 한다. 21.11.15
/// </summary>
class Base : public delenode, public UseSafePtr
{
public:
	Base();
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 소멸자가 virtual 함수인 클래스는 자식 클래스의 다형성을 보장하는가?")
	virtual ~Base();

	virtual void draw_detail_view();

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 개선 필요")
	////  저장을 목적으로 직렬화하는 메서드를 만들어야 한다. 21.11.15
	//template <typename Payload>
	//void serialize(Payload* payload)
	//{
	//}
};