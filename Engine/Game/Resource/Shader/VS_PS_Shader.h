//#pragma once
//
//#include "_Shader.h"
//#include "../../Engine.h"
//
//class Engine;
//namespace SHADER
//{
//	template <typename VertexType, typename PixelType>
//	class VS_PS_Shader : public Shader
//	{
//	public:
//		//CD3D11_RASTERIZER_DESC rasterizer_desc;
//
//		wstring Vertex_shader_path;
//		wstring Pixel_shader_path;
//
//		D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//		//D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
//
//		SHADER::VertexShader Vertex_shader;
//		SHADER::PixelShader Pixel_shader;
//
//	public:
//		VS_PS_Shader(wstring Vertex_shader_path, wstring Pixel_shader_path) :
//			Vertex_shader_path(Vertex_shader_path), Pixel_shader_path(Pixel_shader_path)
//		{
//			//// rasterizer_desc
//			//rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
//			//rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
//		}
//		virtual ~VS_PS_Shader() {}
//	public:
//		// Material을(를) 통해 상속됨
//		virtual void load_resource(Engine* engine) override
//		{
//			__super::load_resource(engine);
//
//			vector<D3D11_INPUT_ELEMENT_DESC> Input_layout;
//			VertexType::T_get_input_layout(Input_layout);
//			Vertex_shader.initialize(engine->Device.Get(), Vertex_shader_path, Input_layout);
//			Pixel_shader.initialize(engine->Device.Get(), Pixel_shader_path);
//		}
//		virtual void release_resource(Engine* engine) override
//		{
//			__super::release_resource(engine);
//
//			Vertex_shader.free_shader();
//			Pixel_shader.free_shader();
//		}
//		virtual void set_shader(ID3D11DeviceContext* Device_context) override
//		{
//			__super::set_shader(Device_context);
//
//			Device_context->IASetPrimitiveTopology(Topology);
//
//			Device_context->IASetInputLayout(Vertex_shader.GetInputLayout());
//			Device_context->VSSetShader(Vertex_shader.GetShader(), 0, 0);
//			Device_context->PSSetShader(Pixel_shader.GetShader(), 0, 0);
//		}
//		virtual void free_material(ID3D11DeviceContext* Device_context) override
//		{
//			__super::free_material(Device_context);
//
//			Device_context->IASetInputLayout(nullptr);
//			Device_context->VSSetShader(nullptr, 0, 0);
//			Device_context->PSSetShader(nullptr, 0, 0);
//		}
//		virtual void draw_detail_view() override
//		{
//			string Text;
//
//			if (ImGui::CollapsingHeader("VS_PS material"))
//			{
//				ImGuiWindowFlags Window_flags = 0;
//				Window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
//				ImGui::BeginChild("VS_PS material", ImVec2(0, 120), true, Window_flags);
//
//				Text = "Resource name : ";
//				if (this->Registed_name.get_string() == "")
//				{
//					ImGui::Text(Text.c_str());
//				}
//				else
//				{
//					Text += this->Registed_name.get_string();
//					ImGui::Text(Text.c_str());
//				}
//
//				ImGui::NewLine();
//				Text = "Vertex type : "; Text += typeid(VertexType).name();
//
//				ImGui::NewLine();
//				Text = "pixel  type : "; Text += typeid(PixelType).name();
//
//				ImGui::EndChild();
//			}
//		}
//	};
//	typedef SHADER::VS_PS_Shader<BTYPE::VB_MeshVertex, BTYPE::PB_SV_vertex>		MeshShader;
//	typedef SHADER::VS_PS_Shader<BTYPE::VB_SV_vertex, BTYPE::PB_SV_vertex>		SVShader;
//}