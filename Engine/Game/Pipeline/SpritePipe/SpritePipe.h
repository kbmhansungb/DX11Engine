#pragma once
#include "../../Engine.h"
#include "../../Resource/Mesh/_Mesh.h"
#include "../../Resource/Shader/_Shader.h"
#include "../../../Component/Renderer/_ViaRenderer.h"

namespace SHADER
{
	class MeshShader : public Shader
	{
	public:
		struct Raw_MeshShader
		{
			std::wstring											pixel_shader_path;
			CD3D11_RASTERIZER_DESC									rasterizer_desc;
			CD3D11_DEPTH_STENCIL_DESC								depth_stencil_desc;
			D3D11_BLEND_DESC										blend_describe{};
		};
		struct Product_MeshShader
		{
			Microsoft::WRL::ComPtr<ID3D11RasterizerState>			rasterizer_state;
			Microsoft::WRL::ComPtr<ID3D11BlendState>				blend_state;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilState>			depth_stencil_state;
			PixelShader												pixel_shader;
		};
		struct Addition_MeshShader
		{
			D3D11_PRIMITIVE_TOPOLOGY								topology;
			std::map<UINT, SafePtr<TEXTURE::Texture>>				vs_textures;
			std::map<UINT, SafePtr<TEXTURE::Texture>>				ps_textures;
		};
		Raw_MeshShader			raw_ms;
		Product_MeshShader		product_ms;
		Addition_MeshShader		addition_ms;

		MeshShader();
		virtual void draw_detail_view() override;

		virtual void load_resource(Engine* engine) override;
		virtual void release_resource(Engine* engine) override;

		void set_mesh_shader(SubGraphics* sub_graphics);
		void set_texture(SubGraphics* sub_graphics);
	};
	REGIST_RESOURCE(MeshShader);
}

namespace MESH
{
	class SpriteMesh : public Mesh
	{
	public:
		// resource
		SafePtr<SHADER::MeshShader>				default_shader;
		std::vector<BTYPE::Vertex_default>		sprite_vertex;
		std::vector<BTYPE::IB_Default>			sprite_index;
		// result
		VertexBuffer<BTYPE::Vertex_default>		buffer_sprite_vertex;
		IndexBuffer<BTYPE::IB_Default>			buffer_sprite_index;


		virtual ~SpriteMesh();
		virtual std::type_index get_class_type() override;

		virtual void load_resource(Engine* engine) override;
		virtual void release_resource(Engine* engine) override;

		void set_sprite_mesh(SubGraphics* sub_graphics, UINT offset = 0);
		void draw_sprite_mesh(SubGraphics* sub_graphics);

		virtual void draw_detail_view() override;
	};
	REGIST_RESOURCE(SpriteMesh);
	SafePtr<MESH::SpriteMesh> default_sprite_mesh(Engine* engine);
}

class SpritePipe
{
public:
	SafePtr<SHADER::MeshShader>	sprite_shader;
	SafePtr<MESH::SpriteMesh> sprite_mesh;
	
	bool is_draw_able();
	void draw(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world);
	void draw_mesh_only(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world);
	static void set_sprite_vs(SubGraphics* sub_graphics);
};