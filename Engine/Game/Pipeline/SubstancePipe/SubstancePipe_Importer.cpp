#include "SubstancePipe_Importer.h"

//////
// RESOURCE_IMPORT_SYSTEM
//////

RESOURCE_IMPORT_SYSTEM::Substance_import::Substance_import() : ResourceImporter("Substance")
{

}

void RESOURCE_IMPORT_SYSTEM::Substance_import::import_file(Engine* engine, std::string path)
{
	Stack stack;
	stack.engine = engine;
	stack.file_path = path;
	Assimp::Importer importer; stack.ai_scene = importer.ReadFile
	(
		path,
		aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded
	);

	if (stack.ai_scene == nullptr)
	{
		ErrorLogger::Log("Failed to load ai scene.");
		return;
	}

	process_textures(&stack);
	process_shaders(&stack);
	process_meshs(&stack);
	process_animation(&stack);
}

void RESOURCE_IMPORT_SYSTEM::Substance_import::process_textures(Stack* stack)
{
	for (UINT index = 0; index < stack->ai_scene->mNumTextures; ++index)
	{
		// 텍스쳐 생성
		// 현재로써는 파일안에 저장된 텍스쳐만 읽을 수 있음. (압축되지 않은 것만 가능한걸로 알고있음)
		std::unique_ptr<TEXTURE::CopiedImage> texture =
			std::make_unique<TEXTURE::CopiedImage>
			(
				stack->file_path,
				reinterpret_cast<uint8_t*>(stack->ai_scene->mTextures[index]->pcData),
				stack->ai_scene->mTextures[index]->mWidth
				);

		// 텍스쳐 등록 종료
		texture->load_resource(stack->engine);
		stack->textures.push_back(
			stack->engine->resources.add_resource(
				NAME::Name(&stack->engine->name_container, stack->file_path + "\\Texture_" + std::to_string(index)),
				texture
			).cast<TEXTURE::Texture>()
		);
	}
}

void RESOURCE_IMPORT_SYSTEM::Substance_import::process_shaders(Stack* stack)
{
	for (UINT index = 0; index < stack->ai_scene->mNumMaterials; ++index)
	{
		// 쉐이더 생성후 초기 설정
		std::unique_ptr<SHADER::MeshShader> shader
			= std::make_unique<SHADER::MeshShader>();
		shader->raw_ms.pixel_shader_path = L"ps_Rendering";
		shader->raw_ms.rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

		// ai 머티리얼에서 사용하는 texture 정보 가져온후 텍스쳐 정보 입력
		aiMaterial* ai_material = stack->ai_scene->mMaterials[index];
		for (
			int texture_index = 0;
			static_cast<UINT>(texture_index) < ai_material->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE);
			++texture_index
			)
		{
			aiString path;
			ai_material->GetTexture(
				aiTextureType::aiTextureType_DIFFUSE,
				texture_index,
				&path
			);

			//int use_texture_index = atoi(&path.C_Str()[1]);
			shader->addition_ms.ps_textures[0] = stack->textures[index];
		}

		// 쉐이더 등록후 종료
		shader->load_resource(stack->engine);
		stack->shaders.push_back(
			stack->engine->resources.add_resource(
				NAME::Name(&stack->engine->name_container, stack->file_path + "\\Shader_" + std::to_string(index)),
				shader
			)
		);
	}
}

void RESOURCE_IMPORT_SYSTEM::Substance_import::process_skeleton(int index, Stack* stack, aiMesh* ai_mesh, std::vector<BoneWeights>* Bone_weights)
{
	// 스켈레톤 생성
	std::unique_ptr<SKELETON::Skeleton> skeleton = std::make_unique<SKELETON::Skeleton>();
	//skeleton->add_new_bone("Root bone", 0, XMMatrixIdentity());

	for (UINT bone_index = 0; bone_index < ai_mesh->mNumBones; ++bone_index)
	{
		aiBone* now_ai_bone = ai_mesh->mBones[bone_index];
		UINT skeleton_bone_index = bone_index;
		//// 본 생성

		// 스켈레톤 본 인덱스와 현재 본의 Offset을 입력.
		XMMATRIX mat = XMMATRIX(&now_ai_bone->mOffsetMatrix.a1);
		mat = XMMatrixInverse(nullptr, mat);
		mat = XMMatrixTranspose(mat);

		SKELETON::Skeleton::Bone* now_bone
			= skeleton->add_new_bone(
				now_ai_bone->mName.C_Str(),
				skeleton_bone_index,
				mat
			);

		aiNode* now_ai_bone_node = stack->ai_scene->mRootNode->FindNode(now_ai_bone->mName);
		// 부모 본이 있다면
		// assimp에서 찾아서 할당해야 한다.
		// 참고로 계층 구조로 찾는 방법이다. 느릴 수 있다.
		
		// 정상적이라면 부모가 nullptr이 아니여야함. 
		// assimp에서 node 계층 구조를 따르기 때문.
		aiNode* parent_ai_bone_node = now_ai_bone_node->mParent; 
		if (skeleton->Name_bone_map.find(parent_ai_bone_node->mName.C_Str()) != skeleton->Name_bone_map.end())
		{
			// 본이 있을 경우. 낮은 순위부터 순차적으로 본이 존재해야 정상적으로 할당 가능. 아마 그렇게 되어 있을 것이다.
			SKELETON::Skeleton::Bone* parent_bone = &skeleton->Bones[skeleton->Name_bone_map[parent_ai_bone_node->mName.C_Str()]];
			skeleton->make_world(now_bone->Bone_index, parent_bone->Bone_index);
		}
		else
		{
			// Assimp에서 Root bone일 경우 Skeleton의 Root로 맵핑시켜 준다.
			skeleton->make_world(now_bone->Bone_index, 0);
		}

		// 버텍스에 본 웨이트 입력
		for (UINT Weight_index = 0; Weight_index < ai_mesh->mBones[bone_index]->mNumWeights; ++Weight_index)
		{
			(*Bone_weights)[ai_mesh->mBones[bone_index]->mWeights[Weight_index].mVertexId]
				.add_bone_weight(
					skeleton_bone_index,														// Bone index
					ai_mesh->mBones[bone_index]->mWeights[Weight_index].mWeight			// Vertex weight
				);
		}
	}

	// 스켈레톤 메쉬 등록후 종료
	skeleton->load_resource(stack->engine);
	stack->skeletons.push_back(
		stack->engine->resources.add_resource(
			NAME::Name(&stack->engine->name_container, stack->file_path + "\\Skeleton_" + std::to_string(index)),
			skeleton
		)
	);

}

// Skeleton은 Mesh에 skeleton이 있을 때 생성됨.

void RESOURCE_IMPORT_SYSTEM::Substance_import::process_meshs(Stack* stack)
{
	for (SIZE_T index = 0; index < stack->ai_scene->mNumMeshes; ++index)
	{
		//////
		// 준비

		std::unique_ptr<MESH::SubstanceMesh> mesh =
			std::make_unique<MESH::SubstanceMesh>();
		size_t vertex_size = stack->ai_scene->mMeshes[index]->mNumVertices;

		//////
		// 정리

		// 메쉬의 버텍스 정의
		auto& vertices = mesh->vertex;
		auto& substancs = mesh->substance;
		vertices.resize(stack->ai_scene->mMeshes[index]->mNumVertices);
		substancs.resize(vertices.size());
		// 포지션 입력, 노말 입력. (이 두개는 항상 존재함)
		for (UINT i = 0; i < stack->ai_scene->mMeshes[index]->mNumVertices; ++i)
		{
			vertices[i].Position.x = stack->ai_scene->mMeshes[index]->mVertices[i].x;
			vertices[i].Position.y = stack->ai_scene->mMeshes[index]->mVertices[i].y;
			vertices[i].Position.z = stack->ai_scene->mMeshes[index]->mVertices[i].z;

			vertices[i].Normal.x = stack->ai_scene->mMeshes[index]->mNormals[i].x;
			vertices[i].Normal.y = stack->ai_scene->mMeshes[index]->mNormals[i].y;
			vertices[i].Normal.z = stack->ai_scene->mMeshes[index]->mNormals[i].z;
		}
		// 텍스쳐 콜디네이트 입력
		if (stack->ai_scene->mMeshes[index]->mTextureCoords[0] != nullptr)
			for (UINT i = 0; i < stack->ai_scene->mMeshes[index]->mNumVertices; ++i)
			{
				vertices[i].Texture_coordinate.x = (float)stack->ai_scene->mMeshes[index]->mTextureCoords[0][i].x;
				vertices[i].Texture_coordinate.y = (float)stack->ai_scene->mMeshes[index]->mTextureCoords[0][i].y;
			}
		// 버텍스 칼라 입력
		if (stack->ai_scene->mMeshes[index]->mColors[0] != nullptr)
			for (UINT i = 0; i < stack->ai_scene->mMeshes[index]->mNumVertices; ++i)
			{
				substancs[i].VertexColor.x = stack->ai_scene->mMeshes[index]->mColors[i]->r;
				substancs[i].VertexColor.y = stack->ai_scene->mMeshes[index]->mColors[i]->g;
				substancs[i].VertexColor.z = stack->ai_scene->mMeshes[index]->mColors[i]->b;
				substancs[i].VertexColor.w = stack->ai_scene->mMeshes[index]->mColors[i]->a;
			}

		// 본 웨이트 정리
		auto& bone_id_weights = mesh->bone_id_weights;
		bone_id_weights.resize(vertex_size);
		if (stack->ai_scene->mMeshes[index]->HasBones())
		{
			// 본이 있다면 다음과 같이 처리

			// skeleton 생성과 본 웨이트 정보 가져오기
			std::vector<BoneWeights> Bone_weights; 
			Bone_weights.resize(stack->ai_scene->mMeshes[index]->mNumVertices);
			process_skeleton(index, stack, stack->ai_scene->mMeshes[index], &Bone_weights);

			for (UINT vertex_index = 0; vertex_index < stack->ai_scene->mMeshes[index]->mNumVertices; ++vertex_index)
			{
				Bone_weights[vertex_index].Normalize();
				for (int i = 0; i < DEFINE_MAX_BONE_LINK_SIZE; ++i)
				{
					bone_id_weights[vertex_index].Bone_index[i] = Bone_weights[vertex_index].Bone_IDs[i];
					bone_id_weights[vertex_index].Bone_Weight[i] = Bone_weights[vertex_index].Bone_weights[i];
				}
				//bone_id_weights[vertex_index].normalize();
			}

			mesh->default_skeleton = stack->skeletons[index];
		}
		else
		{
			// 기본 본을 생성함.
			SafePtr<SKELETON::Skeleton> skeleton_ptr
				= stack->engine->resources.get_resource(Name(&stack->engine->name_container, "_Default_skeleton")).cast<SKELETON::Skeleton>();
			if (skeleton_ptr.is_vaild() == false)
			{
				// 기본 스켈레톤 메쉬가 없으면 재할당함.
				std::unique_ptr<SKELETON::Skeleton> skeleton = std::make_unique<SKELETON::Skeleton>();
				skeleton->add_new_bone("Root bone", 0, XMMatrixIdentity());

				// 스켈레톤 메쉬 등록
				skeleton_ptr = stack->engine->resources.add_resource(
					NAME::Name(&stack->engine->name_container, "_Default_skeleton"),
					skeleton
				);
			}
			stack->skeletons.push_back(skeleton_ptr);

			for (UINT vertex_index = 0; vertex_index < stack->ai_scene->mMeshes[index]->mNumVertices; ++vertex_index)
			{
				bone_id_weights[vertex_index].Bone_Weight[0] = 1.0f;
			}

			mesh->default_skeleton = stack->skeletons[index];
		}

		// bone 정보를 바탕으로 상대좌표로 재정리.
		// 왜 인버스를 구하더라..
		// 안하면 일자로 나옴..
		for (UINT vertex_index = 0; vertex_index < stack->ai_scene->mMeshes[index]->mNumVertices; ++vertex_index)
		{
			XMVECTOR vertex_position{ 0.0f, 0.0f, 0.0f, 1.0f };
			UINT bone_index{};
			float bone_weight{};
			XMMATRIX bone_inverse_mat{};
			// inverse matrix 구하기

			for (int i = 0; i < DEFINE_MAX_BONE_LINK_SIZE; ++i)
			{
				bone_index = bone_id_weights[vertex_index].Bone_index[i];
				bone_weight = bone_id_weights[vertex_index].Bone_Weight[i];
				if (bone_weight != 0.f)
					bone_inverse_mat
						+= stack->skeletons[index]->Bones[bone_index].World_transform
						* bone_weight;
			}

			bone_inverse_mat = XMMatrixInverse(nullptr, bone_inverse_mat);

			// vertex position 구하기
			vertex_position.m128_f32[0] = vertices[vertex_index].Position.x;
			vertex_position.m128_f32[1] = vertices[vertex_index].Position.y;
			vertex_position.m128_f32[2] = vertices[vertex_index].Position.z;

			// 상대좌표 구하고 적용하기.
			vertex_position = XMVector3Transform(vertex_position, bone_inverse_mat);
			vertices[vertex_index].Position.x = vertex_position.m128_f32[0];
			vertices[vertex_index].Position.y = vertex_position.m128_f32[1];
			vertices[vertex_index].Position.z = vertex_position.m128_f32[2];
		}


		//////
		// 준비

		//size_t index_size = stack->ai_scene->mMeshes[index]->mNumFaces;
		//indices.resize(index_size);

		//////
		// 정리

		// 인덱스 정리
		auto& indices = mesh->index;
		indices.clear();
		for (UINT i = 0; i < stack->ai_scene->mMeshes[index]->mNumFaces; ++i)
		{
			aiFace face = stack->ai_scene->mMeshes[index]->mFaces[i];
			for (UINT j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// 메시의 쉐이더 설정
		mesh->default_shader = stack->shaders[stack->ai_scene->mMeshes[index]->mMaterialIndex];

		// 메쉬 등록후 종료
		mesh->load_resource(stack->engine);
		stack->meshs.push_back(
			stack->engine->resources.add_resource(
				NAME::Name(&stack->engine->name_container, stack->file_path + "\\Mesh_" + std::to_string(index)),
				mesh
			)
		);
	}
}

void RESOURCE_IMPORT_SYSTEM::Substance_import::process_animation(Stack* stack)
{
	for (UINT i = 0; i < stack->ai_scene->mNumAnimations; ++i)
	{
		std::unique_ptr<ANIMATION::Animation> animation = std::make_unique<ANIMATION::Animation>();
		aiAnimation* srcAnim = stack->ai_scene->mAnimations[i];

		// name을 가져오고
		std::string animation_name = srcAnim->mName.C_Str();
		animation->duration = static_cast<float>(srcAnim->mDuration);
		animation->tick_per_sec = static_cast<float>(srcAnim->mTicksPerSecond);

		animation->key_frames.resize(srcAnim->mNumChannels);
		for (UINT j = 0; j < srcAnim->mNumChannels; ++j)
		{
			ANIMATION::Animation::KeyFrame& key_frame = animation->key_frames[j];
			aiNodeAnim* srcNodeAnim = srcAnim->mChannels[j];

			// key_frame에서 본의 이름 저장해주고.
			key_frame.bone_name = srcNodeAnim->mNodeName.C_Str();
			// scale과 rotation과 position을 저장해 준다.
			key_frame.scales.resize(srcNodeAnim->mNumScalingKeys);
			for (UINT k = 0; k < srcNodeAnim->mNumScalingKeys; ++k)
			{
				key_frame.scales[k].time = static_cast<float>(srcNodeAnim->mScalingKeys[k].mTime);
				key_frame.scales[k].scale = XMFLOAT3(
					srcNodeAnim->mScalingKeys[k].mValue.x,
					srcNodeAnim->mScalingKeys[k].mValue.y,
					srcNodeAnim->mScalingKeys[k].mValue.z
				);
			}
			key_frame.rotations.resize(srcNodeAnim->mNumRotationKeys);
			for (UINT k = 0; k < srcNodeAnim->mNumRotationKeys; ++k)
			{
				key_frame.rotations[k].time = static_cast<float>(srcNodeAnim->mRotationKeys[k].mTime);
				key_frame.rotations[k].rot = XMFLOAT4(
					srcNodeAnim->mRotationKeys[k].mValue.x,
					srcNodeAnim->mRotationKeys[k].mValue.y,
					srcNodeAnim->mRotationKeys[k].mValue.z,
					srcNodeAnim->mRotationKeys[k].mValue.w
				);
			}
			key_frame.positions.resize(srcNodeAnim->mNumPositionKeys);
			for (UINT k = 0; k < srcNodeAnim->mNumPositionKeys; ++k)
			{
				key_frame.positions[k].time = static_cast<float>(srcNodeAnim->mPositionKeys[k].mTime);
				key_frame.positions[k].pos = XMFLOAT3(
					srcNodeAnim->mPositionKeys[k].mValue.x,
					srcNodeAnim->mPositionKeys[k].mValue.y,
					srcNodeAnim->mPositionKeys[k].mValue.z
				);
			}
		}

		animation->load_resource(stack->engine);
		stack->engine->resources.add_resource(
			NAME::Name(&stack->engine->name_container, stack->file_path + "\\Mesh_" + std::to_string(i) + "_" + animation_name),
			animation
		);
	}
}

void RESOURCE_IMPORT_SYSTEM::Substance_import::BoneWeights::add_bone_weight(UINT Bone_ID, float Bone_weight)
{
	for (UINT index = 0; index < DEFINE_MAX_BONE_LINK_SIZE; ++index)
	{
		if (this->Bone_weights[index] == 0.0f)
		{
			this->Bone_IDs[index] = Bone_ID;
			this->Bone_weights[index] = Bone_weight;
			return;
		}
	}
}

void RESOURCE_IMPORT_SYSTEM::Substance_import::BoneWeights::Normalize()
{
	float total = 0.0f;
	for (float weight : this->Bone_weights)
	{
		total += weight;
	}
	if (total == 0.0f) return;
	for (UINT index = 0; index < DEFINE_MAX_BONE_LINK_SIZE; ++index)
	{
		if (this->Bone_weights[index] != 0.0f)
		{
			this->Bone_weights[index] /= total;
		}
	}
}