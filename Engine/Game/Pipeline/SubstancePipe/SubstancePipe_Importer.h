#pragma once

#include "SubstancePipe_Renderer.h"
#include "../../../Template/Editor_FreeView/Editor_FreeView.h"

namespace RESOURCE_IMPORT_SYSTEM
{
	// Substance는 fbx확장자를 이용한다.
	class Substance_import : public ResourceImporter
	{
	public:
		struct Stack
		{
			Engine* engine = nullptr;
			std::string										file_path;
			const aiScene* ai_scene = nullptr;
			std::vector<SafePtr<TEXTURE::Texture>>			textures;
			std::vector<SafePtr<SHADER::MeshShader>>		shaders;
			std::vector<SafePtr<MESH::SubstanceMesh>>		meshs;
			std::vector<SafePtr<SKELETON::Skeleton>>		skeletons;
		};
		Substance_import();
		virtual void import_file(Engine* engine, std::string path);
		struct BoneWeights
		{
			UINT	Bone_IDs[DEFINE_MAX_BONE_LINK_SIZE]{};
			float	Bone_weights[DEFINE_MAX_BONE_LINK_SIZE]{};

			void add_bone_weight(UINT Bone_ID, float Bone_weight);
			void Normalize();
		};

		void process_textures(
			Stack* stack
		);
		void process_shaders(
			Stack* stack
		);
		void process_skeleton(
			int index,
			Stack* stack,
			aiMesh* ai_mesh,
			std::vector<BoneWeights>* Bone_weights
		);
		// Skeleton은 Mesh에 skeleton이 있을 때 생성됨.
		void process_meshs(
			Stack* stack
		);
		void process_animation(
			Stack* stack
		);
	};
	static Substance_import instance;
}