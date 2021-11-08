//#pragma once
//
//#include "_Shader.h"
//
//namespace SHADER
//{
//	class SkinShader : public Shader
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
//		SkinShader(wstring Vertex_shader_path, wstring Pixel_shader_path);
//		virtual ~SkinShader();
//	public:
//		// Material을(를) 통해 상속됨
//		virtual void load_resource(Engine* engine) override;
//		virtual void release_resource(Engine* engine) override;
//		virtual void set_shader(ID3D11DeviceContext* Device_context) override;
//		virtual void free_material(ID3D11DeviceContext* Device_context) override;
//		virtual void draw_detail_view() override;
//	};
//}