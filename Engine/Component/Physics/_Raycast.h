//#pragma once
//
//#include "../_Include.h"
//
//class Collider;
//class Display;
//class Camera;
//
//class RayTracingLib
//{
//public:
//	class ScreenPosition;
//	class Ray;
//	class HitResult;
//
//	class MousePosition
//	{
//	public:
//		float mouse_x;
//		float mouse_y;
//	public:
//		MousePosition(float mouse_x, float mouse_y);
//	public:
//		Ray to_ray(SafePtr<Display> Display, SafePtr<Camera> Camera);
//	};
//	class Ray
//	{
//	public:
//		XMVECTOR ray_origin;
//		XMVECTOR ray_dir;
//	public:
//		Ray(XMVECTOR ray_origin, XMVECTOR ray_dir) :
//			ray_origin(ray_origin), ray_dir(ray_dir)
//		{}
//		HitResult to_hit_result_by_triangle(const XMVECTOR& p1, const XMVECTOR& p2, const XMVECTOR& p3, const float ray_distance);
//		
//		template <typename BTYPE_Vertex, typename BTYPE_Index>
//		HitResult to_hit_result_by_buffer_data(const std::vector<BTYPE_Vertex>& vertex_data, const std::vector<BTYPE_Index>& index_data, const XMMATRIX& world_transfrom, const float ray_distance)
//		{
//			HitResult Result;
//			Result.Position = ray_origin + ray_dir * ray_distance;
//			Result.distance = ray_distance;
//
//			for (SIZE_T index = 0; index < index_data.size(); index += 3)
//			{
//				HitResult Now_result = to_hit_result_by_triangle(
//					//p1, p2, p3,
//					vertex_data[index_data[index].T_get_index()].T_get_position_vector(),
//					vertex_data[index_data[index + 1].T_get_index()].T_get_position_vector(),
//					vertex_data[index_data[index + 2].T_get_index()].T_get_position_vector(),
//					ray_distance);
//
//				if (Now_result.In_plane && Now_result.distance < Result.distance)
//				{
//					Result = Now_result;
//				}
//			}
//			return Result;
//		}
//		HitResult to_hit_result_by_renderer(Collider* collider, const float ray_distance);
//	};
//	class HitResult
//	{
//	public:
//		bool In_plane;
//		XMVECTOR Position;
//		float distance;
//	};
//	// Screen position -> Ray -> Hit_result
//
//	static void empty() {}
//};
//
//class RayTracing : public Component
//{
//public:
//	SafePtr<WindowInputContainer> Ptr_window_input_container;
//	SafePtr<Display> Ptr_display;
//	SafePtr<Camera> Ptr_camera;
//
//	delegate<RayTracingLib::Ray&> Delegate_ray_cast;
//public:
//	RayTracing(
//		SafePtr<WindowInputContainer> Ptr_window_input_container, 
//		SafePtr<Display> Ptr_display, 
//		SafePtr<Camera> Ptr_camera);
//public:
//	virtual void awake() override;
//	virtual void sleep() override;
//	void Bind_ray_cast(MouseEvent Mouse_event);
//};
//
