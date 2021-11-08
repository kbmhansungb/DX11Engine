//#pragma once
//
//#include "VS_PS_Shader.h"
//
//class TerrainShader : public SHADER::MeshShader
//{
//public:
//	TerrainShader(wstring Vertex_shader_path, wstring Pixel_shader_path) : VS_PS_Shader(Vertex_shader_path, Pixel_shader_path)
//	{}
//	virtual ~TerrainShader() {}
//	SafePtr<TEXTURE::Texture>	Terrain_textures[TERRAIN_TEXTURE_SIZE];
//
//	ID3D11ShaderResourceView* Resource_views[TERRAIN_TEXTURE_SIZE]{};
//	ID3D11SamplerState* Sampler_state[TERRAIN_TEXTURE_SIZE]{};
//
//	void update_terrain_texture()
//	{
//		for (int index = 0; index < TERRAIN_TEXTURE_SIZE; ++index)
//		{
//			if (Terrain_textures[index].is_vaild())
//			{
//				Resource_views[index] = Terrain_textures[index]->GetTextureResourceView();
//				Sampler_state[index] = Terrain_textures[index]->GetSamplerState();
//			}
//			else
//			{
//				Resource_views[index] = nullptr;
//				Sampler_state[index] = nullptr;
//			}
//		}
//	}
//	virtual void load_resource(Engine* engine) override
//	{
//		__super::load_resource(engine);
//		update_terrain_texture();
//	}
//	virtual void set_shader(ID3D11DeviceContext* Device_context) override
//	{
//		__super::set_shader(Device_context);
//
//		Device_context->PSSetShaderResources(10, TERRAIN_TEXTURE_SIZE, Resource_views);
//		Device_context->PSSetSamplers(10, TERRAIN_TEXTURE_SIZE, Sampler_state);
//	}
//};