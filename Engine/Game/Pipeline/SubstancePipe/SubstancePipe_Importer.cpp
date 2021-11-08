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
		// �ؽ��� ����
		// ����ν�� ���Ͼȿ� ����� �ؽ��ĸ� ���� �� ����. (������� ���� �͸� �����Ѱɷ� �˰�����)
		std::unique_ptr<TEXTURE::CopiedImage> texture =
			std::make_unique<TEXTURE::CopiedImage>
			(
				stack->file_path,
				reinterpret_cast<uint8_t*>(stack->ai_scene->mTextures[index]->pcData),
				stack->ai_scene->mTextures[index]->mWidth
				);

		// �ؽ��� ��� ����
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
		// ���̴� ������ �ʱ� ����
		std::unique_ptr<SHADER::MeshShader> shader
			= std::make_unique<SHADER::MeshShader>();
		shader->raw_ms.pixel_shader_path = L"ps_Rendering";
		shader->raw_ms.rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

		// ai ��Ƽ���󿡼� ����ϴ� texture ���� �������� �ؽ��� ���� �Է�
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

		// ���̴� ����� ����
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
	// ���̷��� ����
	std::unique_ptr<SKELETON::Skeleton> skeleton = std::make_unique<SKELETON::Skeleton>();
	//skeleton->add_new_bone("Root bone", 0, XMMatrixIdentity());

	for (UINT bone_index = 0; bone_index < ai_mesh->mNumBones; ++bone_index)
	{
		aiBone* now_ai_bone = ai_mesh->mBones[bone_index];
		UINT skeleton_bone_index = bone_index;
		//// �� ����

		// ���̷��� �� �ε����� ���� ���� Offset�� �Է�.
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
		// �θ� ���� �ִٸ�
		// assimp���� ã�Ƽ� �Ҵ��ؾ� �Ѵ�.
		// ����� ���� ������ ã�� ����̴�. ���� �� �ִ�.
		
		// �������̶�� �θ� nullptr�� �ƴϿ�����. 
		// assimp���� node ���� ������ ������ ����.
		aiNode* parent_ai_bone_node = now_ai_bone_node->mParent; 
		if (skeleton->Name_bone_map.find(parent_ai_bone_node->mName.C_Str()) != skeleton->Name_bone_map.end())
		{
			// ���� ���� ���. ���� �������� ���������� ���� �����ؾ� ���������� �Ҵ� ����. �Ƹ� �׷��� �Ǿ� ���� ���̴�.
			SKELETON::Skeleton::Bone* parent_bone = &skeleton->Bones[skeleton->Name_bone_map[parent_ai_bone_node->mName.C_Str()]];
			skeleton->make_world(now_bone->Bone_index, parent_bone->Bone_index);
		}
		else
		{
			// Assimp���� Root bone�� ��� Skeleton�� Root�� ���ν��� �ش�.
			skeleton->make_world(now_bone->Bone_index, 0);
		}

		// ���ؽ��� �� ����Ʈ �Է�
		for (UINT Weight_index = 0; Weight_index < ai_mesh->mBones[bone_index]->mNumWeights; ++Weight_index)
		{
			(*Bone_weights)[ai_mesh->mBones[bone_index]->mWeights[Weight_index].mVertexId]
				.add_bone_weight(
					skeleton_bone_index,														// Bone index
					ai_mesh->mBones[bone_index]->mWeights[Weight_index].mWeight			// Vertex weight
				);
		}
	}

	// ���̷��� �޽� ����� ����
	skeleton->load_resource(stack->engine);
	stack->skeletons.push_back(
		stack->engine->resources.add_resource(
			NAME::Name(&stack->engine->name_container, stack->file_path + "\\Skeleton_" + std::to_string(index)),
			skeleton
		)
	);

}

// Skeleton�� Mesh�� skeleton�� ���� �� ������.

void RESOURCE_IMPORT_SYSTEM::Substance_import::process_meshs(Stack* stack)
{
	for (SIZE_T index = 0; index < stack->ai_scene->mNumMeshes; ++index)
	{
		//////
		// �غ�

		std::unique_ptr<MESH::SubstanceMesh> mesh =
			std::make_unique<MESH::SubstanceMesh>();
		size_t vertex_size = stack->ai_scene->mMeshes[index]->mNumVertices;

		//////
		// ����

		// �޽��� ���ؽ� ����
		auto& vertices = mesh->vertex;
		auto& substancs = mesh->substance;
		vertices.resize(stack->ai_scene->mMeshes[index]->mNumVertices);
		substancs.resize(vertices.size());
		// ������ �Է�, �븻 �Է�. (�� �ΰ��� �׻� ������)
		for (UINT i = 0; i < stack->ai_scene->mMeshes[index]->mNumVertices; ++i)
		{
			vertices[i].Position.x = stack->ai_scene->mMeshes[index]->mVertices[i].x;
			vertices[i].Position.y = stack->ai_scene->mMeshes[index]->mVertices[i].y;
			vertices[i].Position.z = stack->ai_scene->mMeshes[index]->mVertices[i].z;

			vertices[i].Normal.x = stack->ai_scene->mMeshes[index]->mNormals[i].x;
			vertices[i].Normal.y = stack->ai_scene->mMeshes[index]->mNormals[i].y;
			vertices[i].Normal.z = stack->ai_scene->mMeshes[index]->mNormals[i].z;
		}
		// �ؽ��� �ݵ����Ʈ �Է�
		if (stack->ai_scene->mMeshes[index]->mTextureCoords[0] != nullptr)
			for (UINT i = 0; i < stack->ai_scene->mMeshes[index]->mNumVertices; ++i)
			{
				vertices[i].Texture_coordinate.x = (float)stack->ai_scene->mMeshes[index]->mTextureCoords[0][i].x;
				vertices[i].Texture_coordinate.y = (float)stack->ai_scene->mMeshes[index]->mTextureCoords[0][i].y;
			}
		// ���ؽ� Į�� �Է�
		if (stack->ai_scene->mMeshes[index]->mColors[0] != nullptr)
			for (UINT i = 0; i < stack->ai_scene->mMeshes[index]->mNumVertices; ++i)
			{
				substancs[i].VertexColor.x = stack->ai_scene->mMeshes[index]->mColors[i]->r;
				substancs[i].VertexColor.y = stack->ai_scene->mMeshes[index]->mColors[i]->g;
				substancs[i].VertexColor.z = stack->ai_scene->mMeshes[index]->mColors[i]->b;
				substancs[i].VertexColor.w = stack->ai_scene->mMeshes[index]->mColors[i]->a;
			}

		// �� ����Ʈ ����
		auto& bone_id_weights = mesh->bone_id_weights;
		bone_id_weights.resize(vertex_size);
		if (stack->ai_scene->mMeshes[index]->HasBones())
		{
			// ���� �ִٸ� ������ ���� ó��

			// skeleton ������ �� ����Ʈ ���� ��������
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
			// �⺻ ���� ������.
			SafePtr<SKELETON::Skeleton> skeleton_ptr
				= stack->engine->resources.get_resource(Name(&stack->engine->name_container, "_Default_skeleton")).cast<SKELETON::Skeleton>();
			if (skeleton_ptr.is_vaild() == false)
			{
				// �⺻ ���̷��� �޽��� ������ ���Ҵ���.
				std::unique_ptr<SKELETON::Skeleton> skeleton = std::make_unique<SKELETON::Skeleton>();
				skeleton->add_new_bone("Root bone", 0, XMMatrixIdentity());

				// ���̷��� �޽� ���
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

		// bone ������ �������� �����ǥ�� ������.
		// �� �ι����� ���ϴ���..
		// ���ϸ� ���ڷ� ����..
		for (UINT vertex_index = 0; vertex_index < stack->ai_scene->mMeshes[index]->mNumVertices; ++vertex_index)
		{
			XMVECTOR vertex_position{ 0.0f, 0.0f, 0.0f, 1.0f };
			UINT bone_index{};
			float bone_weight{};
			XMMATRIX bone_inverse_mat{};
			// inverse matrix ���ϱ�

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

			// vertex position ���ϱ�
			vertex_position.m128_f32[0] = vertices[vertex_index].Position.x;
			vertex_position.m128_f32[1] = vertices[vertex_index].Position.y;
			vertex_position.m128_f32[2] = vertices[vertex_index].Position.z;

			// �����ǥ ���ϰ� �����ϱ�.
			vertex_position = XMVector3Transform(vertex_position, bone_inverse_mat);
			vertices[vertex_index].Position.x = vertex_position.m128_f32[0];
			vertices[vertex_index].Position.y = vertex_position.m128_f32[1];
			vertices[vertex_index].Position.z = vertex_position.m128_f32[2];
		}


		//////
		// �غ�

		//size_t index_size = stack->ai_scene->mMeshes[index]->mNumFaces;
		//indices.resize(index_size);

		//////
		// ����

		// �ε��� ����
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

		// �޽��� ���̴� ����
		mesh->default_shader = stack->shaders[stack->ai_scene->mMeshes[index]->mMaterialIndex];

		// �޽� ����� ����
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

		// name�� ��������
		std::string animation_name = srcAnim->mName.C_Str();
		animation->duration = static_cast<float>(srcAnim->mDuration);
		animation->tick_per_sec = static_cast<float>(srcAnim->mTicksPerSecond);

		animation->key_frames.resize(srcAnim->mNumChannels);
		for (UINT j = 0; j < srcAnim->mNumChannels; ++j)
		{
			ANIMATION::Animation::KeyFrame& key_frame = animation->key_frames[j];
			aiNodeAnim* srcNodeAnim = srcAnim->mChannels[j];

			// key_frame���� ���� �̸� �������ְ�.
			key_frame.bone_name = srcNodeAnim->mNodeName.C_Str();
			// scale�� rotation�� position�� ������ �ش�.
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