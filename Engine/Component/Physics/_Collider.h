#pragma once

#include "../_Include.h"
#include "_Raycast.h"

struct Line
{
	enum class TYPE
	{
		STRAIGHT, RAY, LINE,
	};
	TYPE type;
	XMVECTOR origin;
	XMVECTOR vec;
};
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

class Collider : public Component, public ICollider
{
public:
	float radius = 0.0f;

	virtual ~Collider() {}
	virtual void get_hit_result(Line& line, Hit& hit) override;

	// Component을(를) 통해 상속됨
	virtual std::type_index get_class_type() override;
	virtual void draw_detail_view() override;

	// Collider
	// ?????...
	static bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1);

	static void line_sphere_interaction(
		Line& line,
		Hit& hit,
		const XMVECTOR& sphere,
		const float radius
	);

	static void line_triangle_interaction(
		Line& line,
		Hit& hit,
		const XMVECTOR& p1,
		const XMVECTOR& p2,
		const XMVECTOR& p3,
		const bool two_side
	);

	static void line_mesh_interaction(
		Line& line,
		Hit& hit,
		const std::vector<BTYPE::Vertex_default>& vertices,
		const std::vector<BTYPE::IB_Default>& indices,
		const bool two_side
	);
};
REGIST_COMPONENT(Collider);