#pragma once

#include "../_Include.h"

/// <summary>
///  �������� �����ϴ� ������ �⺻ Ŭ����, Base�� ��ӹ޾� �����ؾ� �Ѵ�. 21.11.15
/// </summary>
class Base : public delenode, public UseSafePtr
{
public:
	Base();
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: �Ҹ��ڰ� virtual �Լ��� Ŭ������ �ڽ� Ŭ������ �������� �����ϴ°�?")
	virtual ~Base();

	virtual void draw_detail_view();

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: ���� �ʿ�")
	////  ������ �������� ����ȭ�ϴ� �޼��带 ������ �Ѵ�. 21.11.15
	//template <typename Payload>
	//void serialize(Payload* payload)
	//{
	//}
};