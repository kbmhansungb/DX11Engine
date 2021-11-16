#include "_Collider.h"


void Collider::get_hit_result(Line& line, Hit& hit)
{
	line_sphere_interaction(line, hit, this->owner->pos, radius);
}

void Collider::draw_detail_view()
{
	Component::draw_detail_view();

	if (ImGui::CollapsingHeader("Collider"))
	{
		// Val_Collider 프로퍼티
		std::string name = "radius##" + StringHelper::ptr_to_string(&radius);
		ImGui::InputFloat(name.c_str(), &radius);
	}
}

bool Collider::solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1)
{
	// 충돌을 검사하는 수학적인 방법..

	float discr = b * b - 4.0f * a * c;
	if (discr < 0) return false;
	else if (discr == 0) x0 = x1 = -0.5f * b / a;
	else {
		float q = (b > 0) ?
			-0.5f * (b + sqrt(discr)) :
			-0.5f * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}

void Collider::line_sphere_interaction(
	Line& line, 
	Hit& hit, 
	const XMVECTOR& sphere, 
	const float radius
)
{
	// analytic solution.. ??
	float t0, t1;

	XMVECTOR sub = line.origin - sphere;
	float a = DirectX::XMVector3Dot(line.vec, line.vec).m128_f32[0];
	float b = 2 * DirectX::XMVector3Dot(line.vec, sub).m128_f32[0];
	float c = DirectX::XMVector3Dot(sub, sub).m128_f32[0] - radius * radius;

	if (!solveQuadratic(a, b, c, t0, t1)) goto NOHIT;

	switch (line.type)
	{
	case Line::TYPE::STRAIGHT:
	{
		t0 = abs(t0); t1 = abs(t1);

		if (t0 > t1) std::swap(t0, t1);

		goto HIT;
	}
		break;

	case Line::TYPE::RAY:
	{
		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0) goto NOHIT; // both t0 and t1 are negative 
		}

		goto HIT;
	}
		break;
	case Line::TYPE::LINE:
	{
		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0) goto NOHIT; // both t0 and t1 are negative 
		}

		// 선의 범위를 벗어나면 종료
		if (t0 > 1.0f) goto NOHIT;

		goto HIT;
	}
		break;
	}

	NOHIT:
	{
		// 교점이 없다면 0을 반환한다.
		hit.is_hit = false;
		hit.distance = 0.0f;
		hit.position = XMVECTOR();
		return;
	}
	HIT:
	{
		hit.is_hit = true;
		hit.distance = t0;
		hit.position = line.origin + line.vec * t0;
	}
}

void Collider::line_triangle_interaction(
	Line& line,
	Hit& hit,
	const XMVECTOR& p1,
	const XMVECTOR& p2,
	const XMVECTOR& p3,
	const bool two_side
)
{
	// sphere와 다르게 plane과의 충돌은 반드시 존재함.
	
	// hit point
	XMVECTOR plane = XMPlaneFromPoints(p1, p2, p3);
	hit.position = XMPlaneIntersectLine(plane, line.origin, line.origin + line.vec);
	hit.distance = XMVector3Length(hit.position - line.origin).m128_f32[0];

	// in plane
	XMVECTOR Nor = XMVector3Cross(p3 - p1, p2 - p1);
	XMVECTOR edge0_vec = p2 - p1;
	XMVECTOR edge1_vec = p3 - p2;
	XMVECTOR edge2_vec = p1 - p3;
	XMVECTOR Cross0 = XMVector3Cross(hit.position - p1, edge0_vec);
	XMVECTOR Cross1 = XMVector3Cross(hit.position - p2, edge1_vec);
	XMVECTOR Cross2 = XMVector3Cross(hit.position - p3, edge2_vec);

	// 일단은 이상태로 유지.
	float dot_0 = XMVector3Dot(Nor, Cross0).m128_f32[0];
	float dot_1 = XMVector3Dot(Nor, Cross1).m128_f32[0];
	float dot_2 = XMVector3Dot(Nor, Cross2).m128_f32[0];
	if (
		dot_0 >= 0 &&
		dot_1 >= 0 &&
		dot_2 >= 0
		)
	{
		hit.is_hit = true;
	}
	else
	{
		hit.is_hit = false;
	}

}

void Collider::line_mesh_interaction(
	Line& line, 
	Hit& hit, 
	const std::vector<BTYPE::Vertex_default>& vertices, 
	const std::vector<BTYPE::IB_Default>& indices,
	const bool two_side
)
{
	hit.is_hit = false;
	hit.distance = D3D11_FLOAT32_MAX;
	hit.position = XMVECTOR();

	Hit result;
	size_t end_index = indices.size() / 3;
	XMVECTOR p1, p2, p3;
	for (size_t index = 0; index < end_index; ++index)
	{
		p1 = vertices[indices[index * 3 + 0].Index].T_get_position_vector();
		p2 = vertices[indices[index * 3 + 1].Index].T_get_position_vector();
		p3 = vertices[indices[index * 3 + 2].Index].T_get_position_vector();
		Collider::line_triangle_interaction(
			line, result, p1, p2, p3, two_side
		);

		if (result.is_hit)
		{
			if (hit.distance > result.distance)
			{
				hit = result;
			}
		}
	}
}

