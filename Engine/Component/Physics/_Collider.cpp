#include "_Collider.h"


void Collider::get_hit_result(Line& line, Hit& hit)
{
	line_sphere_interaction(line, hit, this->owner->pos, radius);
}

std::type_index Collider::get_class_type()
{
	return typeid(Collider);
}

void Collider::draw_detail_view()
{
	Component::draw_detail_view();

	if (ImGui::CollapsingHeader("Collider"))
	{
		std::string name = "radius##" + StringHelper::ptr_to_string(&radius);
		ImGui::InputFloat(name.c_str(), &radius);

		// 테스트
		//ImGui::NewLine();
		
		//// 마우스 포지션
		//auto mpos = ImGui::GetIO().MousePos;
		//std::string mpostext = "Mouse pos : " + std::to_string(mpos.x) + " " + std::to_string(mpos.y);
		//ImGui::Text(mpostext.c_str());

		//// 마우스 포지션 to 스크린 포지션
		//auto vpos = ImGui::GetCurrentWindow()->Pos;
		//std::string vpostext = "Viewport pos : " + std::to_string(vpos.x) + " " + std::to_string(vpos.y);
		//ImGui::Text(vpostext.c_str());

		//auto tbh = ImGui::GetCurrentWindow()->TitleBarHeight();
		//std::string tbhtext = "Titlebar height : " + std::to_string(tbh);
		//ImGui::Text(tbhtext.c_str());

		//auto rmpos = ImVec2(mpos.x - vpos.x, mpos.x - vpos.x - tbh);
		//std::string rmpostext = "Relative mouse pos : " + std::to_string(rmpos.x) + " " + std::to_string(rmpos.y);
		//ImGui::Text(rmpostext.c_str());


		// 트라이엥글 인터렉션 테스트.
		static Line line{};
		static Hit hit{};
		static XMVECTOR p1{};
		static XMVECTOR p2{};
		static XMVECTOR p3{};

		Collider::line_triangle_interaction(line, hit, p1, p2, p3, true);
		ImGui::NewLine();
		if (ImGui::Button("Straight"))
		{
			line.type = Line::TYPE::STRAIGHT;
		}
		if (ImGui::Button("Ray"))
		{
			line.type = Line::TYPE::RAY;
		}
		if (ImGui::Button("Line"))
		{
			line.type = Line::TYPE::LINE;
		}
		ImGui::XMVECTOR_field(line.origin, "line origion");
		ImGui::XMVECTOR_field(line.vec, "line vec");
		ImGui::NewLine();
		ImGui::XMVECTOR_field(p1, "p1");
		ImGui::XMVECTOR_field(p2, "p2");
		ImGui::XMVECTOR_field(p3, "p3");
		ImGui::NewLine();
		ImGui::XMVECTOR_field(hit.position, "hit position");
		ImGui::InputFloat("hit distance", &hit.distance);
		ImGui::Text(std::to_string(hit.is_hit).c_str());
	}
}


// Collider
// ?????...

bool Collider::solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1)
{
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
	// 상관없음.
	hit.position = XMPlaneIntersectLine(plane, line.origin, line.origin + line.vec);
	//switch (line.type)
	//{
	//case Line::TYPE::LINE:
	//{
	//	hit.position = XMPlaneIntersectLine(plane, line.origin, line.origin + line.vec);
	//}
	//	break;
	//case Line::TYPE::RAY:
	//{
	//	hit.position = XMPlaneIntersectLine(plane, line.origin, line.origin + line.vec * 10000.0f);
	//}
	//	break;
	//case Line::TYPE::STRAIGHT:
	//{
	//	hit.position = XMPlaneIntersectLine(plane, line.origin - line.vec * 10000.0f, line.origin + line.vec * 10000.0f);
	//}
	//	break;
	//}
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

