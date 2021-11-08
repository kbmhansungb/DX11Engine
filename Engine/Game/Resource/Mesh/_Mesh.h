#pragma once
#include "../Resource.h"
#include "../Texture/_Texture.h"
#include "../Shader/_Shader.h"

namespace MESH
{
	class Mesh : public Resource
	{
	public:
		virtual ~Mesh() {}
		virtual std::type_index get_class_type() override { return typeid(Mesh); }

		virtual void load_resource(Engine* engine) override = 0;
		virtual void release_resource(Engine* engine) override = 0;
		virtual void draw_detail_view() override;
	};
}

//// export ¿¹Á¦.
//aiReturn assimp_export(string format, string path)
//{
//	aiScene scene;
//	{
//		scene.mRootNode = new aiNode();
//
//		scene.mMaterials = new aiMaterial * [1];
//		scene.mMaterials[0] = nullptr;
//		scene.mNumMaterials = 1;
//
//		scene.mMaterials[0] = new aiMaterial();
//
//		scene.mMeshes = new aiMesh * [1];
//		scene.mMeshes[0] = nullptr;
//		scene.mNumMeshes = 1;
//
//		scene.mMeshes[0] = new aiMesh();
//		scene.mMeshes[0]->mMaterialIndex = 0;
//
//		scene.mRootNode->mMeshes = new unsigned int[1];
//		scene.mRootNode->mMeshes[0] = 0;
//		scene.mRootNode->mNumMeshes = 1;
//
//	}
//	auto pMesh = scene.mMeshes[0];
//
//	// Create mesh scene
//	{
//		ToolData* data = ToolData::get_map_tool_data();
//		auto have_mesh = data->terrain_renderer->VS_PS_have_mesh;
//		
//		// make vertex
//		{
//			int vertex_size = have_mesh->Vertex_vector.size();
//
//			pMesh->mVertices = new aiVector3D[vertex_size];
//			pMesh->mNormals = new aiVector3D[vertex_size];
//			pMesh->mNumVertices = vertex_size;
//
//			pMesh->mTextureCoords[0] = new aiVector3D[vertex_size];
//			pMesh->mNumUVComponents[0] = vertex_size;
//
//			pMesh->mColors[0] = new aiColor4D[vertex_size];
//
//			for (int index = 0; index < vertex_size; ++index)
//			{
//				pMesh->mVertices[index].x = have_mesh->Vertex_vector[index].Position.x;
//				pMesh->mVertices[index].y = have_mesh->Vertex_vector[index].Position.y;
//				pMesh->mVertices[index].z = have_mesh->Vertex_vector[index].Position.z;
//
//				pMesh->mNormals[index].x = have_mesh->Vertex_vector[index].Normal.x;
//				pMesh->mNormals[index].y = have_mesh->Vertex_vector[index].Normal.y;
//				pMesh->mNormals[index].z = have_mesh->Vertex_vector[index].Normal.z;
//
//				pMesh->mTextureCoords[0][index].x = have_mesh->Vertex_vector[index].Texture_coordinate.x;
//				pMesh->mTextureCoords[0][index].y = have_mesh->Vertex_vector[index].Texture_coordinate.y;
//
//				pMesh->mColors[0][index].r = have_mesh->Vertex_vector[index].VertexColor.x;
//				pMesh->mColors[0][index].g = have_mesh->Vertex_vector[index].VertexColor.y;
//				pMesh->mColors[0][index].b = have_mesh->Vertex_vector[index].VertexColor.z;
//				pMesh->mColors[0][index].a = have_mesh->Vertex_vector[index].VertexColor.w;
//			}
//		}
//
//		// make face
//		{
//			pMesh->mNumFaces = (unsigned int)(have_mesh->Index_vector.size() / 3);
//			pMesh->mFaces = new aiFace[pMesh->mNumFaces];
//
//			for (int index = 0; index < (pMesh->mNumFaces); ++index)
//			{
//				aiFace& face = pMesh->mFaces[index];
//				face.mIndices = new unsigned int[3];
//				face.mNumIndices = 3;
//
//				face.mIndices[0] = have_mesh->Index_vector[index * 3 + 0].Index;
//				face.mIndices[1] = have_mesh->Index_vector[index * 3 + 1].Index;
//				face.mIndices[2] = have_mesh->Index_vector[index * 3 + 2].Index;
//			}
//		}
//	}
//
//	Assimp::Exporter exporter;
//	return exporter.Export(&scene, format.c_str(), path.c_str());
//}