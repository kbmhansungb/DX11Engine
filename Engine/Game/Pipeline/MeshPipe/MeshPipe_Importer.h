#pragma once

#include "MeshPipe.h"

namespace RESOURCE_IMPORT_SYSTEM
{
	class  Meshmesh_import : ResourceImporter
	{
	public:
		struct Stack
		{
			Engine* engine = nullptr;
			std::string										file_path;

			const aiScene* ai_scene = nullptr;
			std::vector<SafePtr<TEXTURE::Texture>>			textures;
			std::vector<SafePtr<SHADER::Shader>>			shaders;
			std::vector<SafePtr<MESH::Mesh>>				meshs;
		};
		Meshmesh_import();

		virtual void import_file(Engine* engine, std::string path);
	};
	static Meshmesh_import Meshmesh_import_instance;

	static void create_textures(Meshmesh_import::Stack* stack);
	static void process_textures(Meshmesh_import::Stack* stack);
	static void load_shaders(Meshmesh_import::Stack* stack, std::string classific);
	static void create_mesh_shaders(
		Meshmesh_import::Stack* stack,
		std::string classific,
		void(*callback)(Meshmesh_import::Stack*, SHADER::MeshShader*, int));
	static void mapping_texture_to_shader(
		Meshmesh_import::Stack* stack,
		SHADER::MeshShader* shader,
		int index);
	static void process_shaders(Meshmesh_import::Stack* stack);
	static void load_meshs(Meshmesh_import::Stack* stack, std::string classific);
	static void create_mesh_meshs(
		Meshmesh_import::Stack* stack,
		std::string classific,
		void(*callback)(Meshmesh_import::Stack*, MESH::Mesh*, int));
	static void insert_mesh_mesh_vertices(
		Meshmesh_import::Stack* stack,
		MESH::Mesh* mesh,
		int mesh_index);
	static void process_meshs(Meshmesh_import::Stack* stack);
}