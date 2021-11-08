#include "MeshPipe_Importer.h"

#include "../../Engine.h"
#include "../../../Component/Graphics/_SubGraphics.h"
#include "../../../Template/Editor_FreeView/Editor_FreeView.h"
#include "../../../Template/Editor_ImageView/Editor_ImageView.h"

RESOURCE_IMPORT_SYSTEM::Meshmesh_import::Meshmesh_import() : ResourceImporter("MeshMesh")
{

}

void RESOURCE_IMPORT_SYSTEM::Meshmesh_import::import_file(Engine* engine, std::string path)
{
	Stack stack;
	stack.engine = engine;
	stack.file_path = path;
	Assimp::Importer importer; stack.ai_scene = importer.ReadFile
	(
		path,
		aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded
	);

	RESOURCE_IMPORT_SYSTEM::create_textures(&stack);
	RESOURCE_IMPORT_SYSTEM::process_textures(&stack);

	RESOURCE_IMPORT_SYSTEM::load_shaders(&stack, "MeshShader");
	RESOURCE_IMPORT_SYSTEM::create_mesh_shaders(&stack, "MeshShader",
		&mapping_texture_to_shader);
	RESOURCE_IMPORT_SYSTEM::process_shaders(&stack);

	RESOURCE_IMPORT_SYSTEM::load_meshs(&stack, "MeshMesh");
	RESOURCE_IMPORT_SYSTEM::create_mesh_meshs(&stack, "MeshMesh",
		&insert_mesh_mesh_vertices);
	RESOURCE_IMPORT_SYSTEM::process_meshs(&stack);
}

void RESOURCE_IMPORT_SYSTEM::create_textures(Meshmesh_import::Stack* stack)
{
	stack->textures.resize(stack->ai_scene->mNumTextures);
	for (UINT index = 0; index < stack->ai_scene->mNumTextures; ++index)
	{
		NAME::Name texture_name = Name(
			&stack->engine->name_container,
			stack->file_path + "\\Texture_" + std::to_string(index)
		);

		SafePtr<TEXTURE::Texture> texture
			= stack->engine->resources.get_resource(texture_name)
			.cast<TEXTURE::Texture>();
		if (texture.is_vaild() == false)
		{
			// 텍스쳐가 없다면 새로 생성함.
			std::unique_ptr<TEXTURE::CopiedImage> texture_uptr =
				std::make_unique<TEXTURE::CopiedImage>
				(
					stack->file_path,
					reinterpret_cast<uint8_t*>(stack->ai_scene->mTextures[index]->pcData),
					stack->ai_scene->mTextures[index]->mWidth
					);

			texture
				= stack->engine->resources.add_resource(texture_name, texture_uptr)
				.cast<TEXTURE::Texture>();
		}
		stack->textures[index] = texture;
	}
}

void RESOURCE_IMPORT_SYSTEM::process_textures(Meshmesh_import::Stack* stack)
{
	for (UINT index = 0; index < stack->ai_scene->mNumTextures; ++index)
	{
		stack->textures[index]->load_resource(stack->engine);
	}
}

void RESOURCE_IMPORT_SYSTEM::load_shaders(Meshmesh_import::Stack* stack, std::string classific)
{
	stack->shaders.resize(stack->ai_scene->mNumMaterials);
	for (UINT index = 0; index < stack->ai_scene->mNumMaterials; ++index)
	{
		NAME::Name shader_name = Name(
			&stack->engine->name_container,
			stack->file_path + "\\" + classific + "_" + std::to_string(index)
		);

		stack->shaders[index]
			= stack->engine->resources.get_resource(shader_name)
			.cast<SHADER::Shader>();
	}
}

void RESOURCE_IMPORT_SYSTEM::create_mesh_shaders(Meshmesh_import::Stack* stack, std::string classific, void(*callback)(Meshmesh_import::Stack*, SHADER::MeshShader*, int))
{
	for (UINT index = 0; index < stack->ai_scene->mNumMaterials; ++index)
	{
		NAME::Name shader_name = Name(
			&stack->engine->name_container,
			stack->file_path + "\\" + classific + "_" + std::to_string(index)
		);

		if (stack->shaders[index].is_vaild() == false)
		{
			// 쉐이더가 없다면 새로 생성함.
			std::unique_ptr<SHADER::MeshShader> shader_uptr
				= std::make_unique<SHADER::MeshShader>();
			shader_uptr->raw_ms.pixel_shader_path = L"ps_Rendering";

			callback(stack, shader_uptr.get(), index);

			// 리소스 등록
			stack->shaders[index]
				= stack->engine->resources.add_resource(shader_name, shader_uptr)
				.cast<SHADER::Shader>();
		}
	}
}

void RESOURCE_IMPORT_SYSTEM::mapping_texture_to_shader(Meshmesh_import::Stack* stack, SHADER::MeshShader* shader, int index)
{
	// index는 assimp에서 머테리얼의 순번을 나타냄. DIFFUSE만을 임포트 하기 때문에..
	//텍스쳐의 순서는 머테리얼의 순서와 같음.
	aiMaterial* ai_material = stack->ai_scene->mMaterials[index];
	for (
		UINT texture_index = 0;
		texture_index < ai_material->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE);
		++texture_index
		)
	{
		aiString path;
		ai_material->GetTexture(
			aiTextureType::aiTextureType_DIFFUSE,
			texture_index,
			&path
		);

		int use_texture_index = atoi(&path.C_Str()[1]);
		shader->addition_ms.ps_textures[texture_index] = stack->textures[index];
	}
}

void RESOURCE_IMPORT_SYSTEM::process_shaders(Meshmesh_import::Stack* stack)
{
	for (UINT index = 0; index < stack->ai_scene->mNumMaterials; ++index)
	{
		stack->shaders[index]->load_resource(stack->engine);
	}
}

void RESOURCE_IMPORT_SYSTEM::load_meshs(Meshmesh_import::Stack* stack, std::string classific)
{
	stack->meshs.resize(stack->ai_scene->mNumMeshes);
	for (SIZE_T index = 0; index < stack->ai_scene->mNumMeshes; ++index)
	{
		NAME::Name mesh_name = Name(
			&stack->engine->name_container,
			stack->file_path + "\\" + classific + "_" + std::to_string(index)
		);

		SafePtr<MESH::MeshMesh> mesh_mesh
			= stack->engine->resources.get_resource(mesh_name)
			.cast<MESH::MeshMesh>();
	}
}

void RESOURCE_IMPORT_SYSTEM::create_mesh_meshs(Meshmesh_import::Stack* stack, std::string classific, void(*callback)(Meshmesh_import::Stack*, MESH::Mesh*, int))
{
	for (SIZE_T index = 0; index < stack->ai_scene->mNumMeshes; ++index)
	{
		NAME::Name mesh_name = Name(
			&stack->engine->name_container,
			stack->file_path + "\\" + classific + "_" + std::to_string(index)
		);

		if (stack->meshs[index].is_vaild() == false)
		{
			std::unique_ptr<MESH::MeshMesh> mesh_mesh_uptr =
				std::make_unique<MESH::MeshMesh>();

			callback(stack, mesh_mesh_uptr.get(), index);

			// 메시의 쉐이더 설정
			stack->meshs[index]
				= stack->engine->resources.add_resource(mesh_name, mesh_mesh_uptr)
				.cast<MESH::Mesh>();
		}
	}
}

void RESOURCE_IMPORT_SYSTEM::insert_mesh_mesh_vertices(Meshmesh_import::Stack* stack, MESH::Mesh* mesh, int mesh_index)
{
	MESH::MeshMesh* mesh_mesh = dynamic_cast<MESH::MeshMesh*>(mesh);
	auto& vertices = mesh_mesh->product_mm.vertex;
	vertices.resize(stack->ai_scene->mMeshes[mesh_index]->mNumVertices);

	// 포지션 입력, 노말 입력. (이 두개는 항상 존재함)
	for (UINT i = 0; i < stack->ai_scene->mMeshes[mesh_index]->mNumVertices; ++i)
	{
		vertices[i].Position.x = stack->ai_scene->mMeshes[mesh_index]->mVertices[i].x;
		vertices[i].Position.y = stack->ai_scene->mMeshes[mesh_index]->mVertices[i].y;
		vertices[i].Position.z = stack->ai_scene->mMeshes[mesh_index]->mVertices[i].z;

		vertices[i].Normal.x = stack->ai_scene->mMeshes[mesh_index]->mNormals[i].x;
		vertices[i].Normal.y = stack->ai_scene->mMeshes[mesh_index]->mNormals[i].y;
		vertices[i].Normal.z = stack->ai_scene->mMeshes[mesh_index]->mNormals[i].z;
	}
	// 텍스쳐 콜디네이트 입력
	if (stack->ai_scene->mMeshes[mesh_index]->mTextureCoords[0] != nullptr)
		for (UINT i = 0; i < stack->ai_scene->mMeshes[mesh_index]->mNumVertices; ++i)
		{
			vertices[i].Texture_coordinate.x = (float)stack->ai_scene->mMeshes[mesh_index]->mTextureCoords[0][i].x;
			vertices[i].Texture_coordinate.y = (float)stack->ai_scene->mMeshes[mesh_index]->mTextureCoords[0][i].y;
		}


	// 인덱스 정리
	auto& indices = mesh_mesh->product_mm.index;
	indices.clear();
	for (UINT i = 0; i < stack->ai_scene->mMeshes[mesh_index]->mNumFaces; ++i)
	{
		aiFace face = stack->ai_scene->mMeshes[mesh_index]->mFaces[i];
		for (UINT j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// 메시의 쉐이더 설정
	mesh_mesh->addition_mm.default_shader
		= stack->shaders[stack->ai_scene->mMeshes[mesh_index]->mMaterialIndex]
		.cast<SHADER::MeshShader>();

}

void RESOURCE_IMPORT_SYSTEM::process_meshs(Meshmesh_import::Stack* stack)
{
	for (SIZE_T index = 0; index < stack->ai_scene->mNumMeshes; ++index)
	{
		stack->meshs[index]->load_resource(stack->engine);
	}
}
