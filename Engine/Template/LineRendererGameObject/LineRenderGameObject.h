//#pragma once
//
//#include "../../Component/Renderer/DirectRenderer.h"
//
//static string Line_3D_shader_name = "Line_3D_shader";
//
//class LineRenderGameObject : public GameObject
//{
//public:
//	SafePtr<DirectRenderer> Direct_renderer;
//public:
//	LineRenderGameObject()
//	{
//		SafePtr<Engine> engine = this->this_scene->this_engine;
//
//		// Direct_renderer 积己.
//		Direct_renderer 
//			= this->add_component(new DirectRenderer());
//
//		//SafePtr<SHADER::VS_PS_Shader<BTYPE::VB_MeshVertex, BTYPE::PB_SV_vertex>> Line_shader;
//		//Line_shader.set(engine->resources.get_resource(NAME::Name(&engine->name_container, Line_3D_shader_name)));
//		//if (Line_shader.is_vaild() == false)
//		//{
//		//	auto unique = 
//		//		make_unique<SHADER::VS_PS_Shader<BTYPE::VB_MeshVertex, BTYPE::PB_SV_vertex>>
//		//		(L"DefaultMesh_vs_shader", L"DefaultMesh_ps_shader");
//		//	Line_shader = engine->resources.add_resource
//		//		(
//		//			NAME::Name(&engine->name_container, Line_3D_shader_name),
//		//			unique
//		//		);
//		//	Line_shader->rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
//		//	Line_shader->rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
//		//	Line_shader->Topology = D3D10_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
//		//	Line_shader->depth_stencil_desc.DepthEnable = false;
//		//	engine->resources.load_resource(engine.get(), Line_shader);
//		//}
//		//Direct_renderer->Mesh_direct_struct.shader = Line_shader;
//		//// Direct renderer 积己 辆丰
//	}
//	virtual ~LineRenderGameObject() {}
//};