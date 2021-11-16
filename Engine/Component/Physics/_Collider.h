#pragma once

#include "../_Include.h"

/// <summary>
/// ���� ������ ������ �ִ� ������ ��Ʈ��Ʈ
/// 21.11.16
/// </summary>
struct Line
{
	/// <summary>
	/// STRAIGHT	: ����
	/// RAY			: ������
	/// LINE		: ��
	/// 21.11.16
	/// </summary>
	enum class TYPE
	{
		STRAIGHT, RAY, LINE,
	};
	TYPE type;
	XMVECTOR origin;
	XMVECTOR vec;
};

/// <summary>
/// ���� ������ ������ �ִ� ������ ��Ʈ��Ʈ
/// 21.11.16
/// </summary>
struct Hit
{
	bool is_hit;
	XMVECTOR position;
	float distance;
};

interface ICollider
{
	virtual void get_hit_result(Line& line, Hit& hit) = 0;
};

/// <summary>
/// �ݶ��̴� ������Ʈ,
/// ���� ó����
/// 21.11.16
/// </summary>
class Collider : public Component, public ICollider
{

#pragma region ConDestructor

public:
	~Collider() {}

#pragma endregion

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: �̿ϼ� �ڵ�")
	// 1. �ݶ��̴��� ���Ǿ� -> ������ �� -> �޽� ������ Ȯ�� �� �� �־����.

	// �ݶ��̴��� ���Ǿ� �������� ���� ����.
	// 21.11.16
#pragma region Val_Collider

public:
	float radius = 0.0f;

#pragma endregion

	// �̿ϼ� �ڵ�
	// 21.11.16
#pragma region Collider

public:
	virtual void get_hit_result(Line& line, Hit& hit) override;

#pragma endregion

#pragma region Component

public:
	virtual void draw_detail_view() override;

#pragma endregion

	// ����ĳ��Ʈ�� ���� ����ƽ �Լ� ����
	// 21.11.16
#pragma region Raycast

	/// <summary>
	/// �⺻ �浹 �˻� �޼���
	/// 21.11.16
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <param name="x0"></param>
	/// <param name="x1"></param>
	/// <returns></returns>
	static bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1);

	/// <summary>
	/// ���ΰ� ��ü�� �浹 �˻�
	/// 21.11.16
	/// </summary>
	/// <param name="line"></param>
	/// <param name="hit"></param>
	/// <param name="sphere"></param>
	/// <param name="radius"></param>
	static void line_sphere_interaction(
		Line& line,
		Hit& hit,
		const XMVECTOR& sphere,
		const float radius
	);

	/// <summary>
	/// ���ΰ� �ﰢ���� �浹 �˻�
	/// 21.11.16
	/// </summary>
	/// <param name="line"></param>
	/// <param name="hit"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <param name="two_side"></param>
	static void line_triangle_interaction(
		Line& line,
		Hit& hit,
		const XMVECTOR& p1,
		const XMVECTOR& p2,
		const XMVECTOR& p3,
		const bool two_side
	);

	/// <summary>
	/// ���ΰ� �޽��� �浹 �˻�
	/// ���ؽ��� �ε��� ������ �ﰢ�� �浹 �˻縦 ������.
	/// 21.11.16
	/// </summary>
	/// <param name="line"></param>
	/// <param name="hit"></param>
	/// <param name="vertices"></param>
	/// <param name="indices"></param>
	/// <param name="two_side"></param>
	static void line_mesh_interaction(
		Line& line,
		Hit& hit,
		const std::vector<BTYPE::Vertex_default>& vertices,
		const std::vector<BTYPE::IB_Default>& indices,
		const bool two_side
	);

#pragma endregion

};
REGIST_COMPONENT(Collider);