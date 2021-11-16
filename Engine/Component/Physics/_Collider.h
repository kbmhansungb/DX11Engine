#pragma once

#include "../_Include.h"

/// <summary>
/// 선의 정보를 가지고 있는 데이터 스트럭트
/// 21.11.16
/// </summary>
struct Line
{
	/// <summary>
	/// STRAIGHT	: 직선
	/// RAY			: 반직선
	/// LINE		: 선
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
/// 츙돌 정보를 가지고 있는 데이터 스트럭트
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
/// 콜라이더 컴포넌트,
/// 힛을 처리함
/// 21.11.16
/// </summary>
class Collider : public Component, public ICollider
{

#pragma region ConDestructor

public:
	~Collider() {}

#pragma endregion

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 미완성 코드")
	// 1. 콜라이더의 스피어 -> 쉐이프 블럭 -> 메쉬 순으로 확인 할 수 있어야함.

	// 콜라이더의 스피어 절반정도 까지 만듬.
	// 21.11.16
#pragma region Val_Collider

public:
	float radius = 0.0f;

#pragma endregion

	// 미완성 코드
	// 21.11.16
#pragma region Collider

public:
	virtual void get_hit_result(Line& line, Hit& hit) override;

#pragma endregion

#pragma region Component

public:
	virtual void draw_detail_view() override;

#pragma endregion

	// 레이캐스트를 위한 스테틱 함수 모음
	// 21.11.16
#pragma region Raycast

	/// <summary>
	/// 기본 충돌 검사 메서드
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
	/// 라인과 구체의 충돌 검사
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
	/// 라인과 삼각형의 충돌 검사
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
	/// 라인과 메쉬의 충돌 검사
	/// 버텍스와 인덱스 정보로 삼각형 충돌 검사를 진행함.
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