//#include "_Raycast.h"
//
//#include "_Collider.h"
//#include "../Display/Display.h"
//#include "../Camera/_Camera.h"
//
//RayTracingLib::HitResult RayTracingLib::Ray::to_hit_result_by_triangle(const XMVECTOR& p1, const XMVECTOR& p2, const XMVECTOR& p3, const float ray_distance)
//{
//	HitResult Hit_result;
//	// hit point
//	XMVECTOR plane = XMPlaneFromPoints(p1, p2, p3);
//	Hit_result.Position = XMPlaneIntersectLine(plane, ray_origin, ray_origin + ray_dir * ray_distance);
//	// in plane
//	XMVECTOR Nor = XMVector3Cross(p3 - p1, p2 - p1);
//	XMVECTOR edge0_vec = p2 - p1;
//	XMVECTOR edge1_vec = p3 - p2;
//	XMVECTOR edge2_vec = p1 - p3;
//	XMVECTOR Cross0 = XMVector3Cross(Hit_result.Position - p1, edge0_vec);
//	XMVECTOR Cross1 = XMVector3Cross(Hit_result.Position - p2, edge1_vec);
//	XMVECTOR Cross2 = XMVector3Cross(Hit_result.Position - p3, edge2_vec);
//
//	float dot_0 = XMVector3Dot(Nor, Cross0).m128_f32[0];
//	float dot_1 = XMVector3Dot(Nor, Cross1).m128_f32[0];
//	float dot_2 = XMVector3Dot(Nor, Cross2).m128_f32[0];
//	if (
//		dot_0 > 0 &&
//		dot_1 > 0 &&
//		dot_2 > 0
//		)
//	{
//		Hit_result.In_plane = true;
//	}
//	else
//	{
//		Hit_result.In_plane = false;
//	}
//
//	Hit_result.distance = XMVector3Length(Hit_result.Position - this->ray_origin).m128_f32[0];
//	return Hit_result;
//}
//
//RayTracingLib::HitResult RayTracingLib::Ray::to_hit_result_by_renderer(Collider* collider, const float ray_distance)
//{
//	RayTracingLib::HitResult hit_result;
//	hit_result.Position = this->ray_origin + this->ray_dir * ray_distance;
//	hit_result.distance = ray_distance;
//	//collider->get_hit_result(this, &hit_result, ray_distance);
//	return hit_result;
//}
//
//RayTracingLib::MousePosition::MousePosition(float mouse_x, float mouse_y) :
//	mouse_x(mouse_x), mouse_y(mouse_y)
//{}
//
//RayTracingLib::Ray RayTracingLib::MousePosition::to_ray(SafePtr<Display> Display, SafePtr<Camera> Camera)
//{
//	// setting
//	int Viewport_width = Display.get()->windowWidth;
//	int Viewport_height = Display.get()->windowHeight;
//	XMMATRIX view_inverse = XMMatrixInverse(nullptr, Camera->get_vIew());
//	XMMATRIX projection_inverse = XMMatrixInverse(nullptr, Camera->get_projection());
//
//	// make sv ray
//	XMVECTOR ray_origin = XMVectorSet(
//		(mouse_x * 2.0f / Viewport_width) - 1.f,//((mouse_x * 2.0f / Viewport_width) - 1.0f) / projection_store(0, 0),
//		(mouse_y * -2.0f / Viewport_height) + 1.f,//((mouse_y * -2.0f / Viewport_height) + 1.0f) / projection_store(1, 1),
//		0.0f,
//		1.0f
//	);
//	XMVECTOR ray_dir = XMVectorSet(
//		(mouse_x * 2.0f / Viewport_width) - 1.f, //((mouse_x * 2.0f / Viewport_width) - 1.0f) / projection_store(0, 0),
//		(mouse_y * -2.0f / Viewport_height) + 1.f, //((mouse_y * -2.0f / Viewport_height) + 1.0f) / projection_store(1, 1),
//		1.0f,
//		1.0f
//	);
//
//	// return ray
//	XMMATRIX To_local_matrix = XMMatrixMultiply(projection_inverse, view_inverse);
//	ray_origin = XMVector3TransformCoord(ray_origin, To_local_matrix);
//	ray_dir = XMVector3TransformCoord(ray_dir, To_local_matrix);
//	ray_dir = XMVector3Normalize(ray_dir - ray_origin);
//	return Ray(ray_origin, ray_dir);
//}
//
//RayTracing::RayTracing(SafePtr<WindowInputContainer> Ptr_window_input_container, SafePtr<Display> Ptr_display, SafePtr<Camera> Ptr_camera) :
//	Ptr_window_input_container(Ptr_window_input_container),
//	Ptr_display(Ptr_display),
//	Ptr_camera(Ptr_camera)
//{}
//
//void RayTracing::awake()
//{
//	Ptr_window_input_container->Delegate_mouse_event.AddInvoker(this, &RayTracing::Bind_ray_cast);
//}
//
//void RayTracing::sleep()
//{
//	Ptr_window_input_container->Delegate_mouse_event.RemoveInvoker(this, &RayTracing::Bind_ray_cast);
//}
//
//void RayTracing::Bind_ray_cast(MouseEvent Mouse_event)
//{
//	if (Mouse_event.GetType() == MouseEventType::LPRESS)
//	{
//		RayTracingLib::MousePosition Mouse_position(static_cast<float>(Mouse_event.GetPosX()), static_cast<float>(Mouse_event.GetPosY()));
//		RayTracingLib::Ray Ray = Mouse_position.to_ray(Ptr_display, Ptr_camera);
//
//		//auto Device = get_owner()->get_scene()->get_engine()->Device.Get();
//		Delegate_ray_cast.Invoke(Ray);
//	}
//}
