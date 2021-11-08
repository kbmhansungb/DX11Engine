#pragma once
#include "../../../Game/_Exlude.h"
#include "../../../Component/Renderer/_ViaRenderer.h"
#include "../MeshPipe/MeshPipe.h"

namespace BTYPE
{	
	struct BoneIDWeights
	{
		UINT  Bone_index[DEFINE_MAX_BONE_LINK_SIZE]{};
		float Bone_Weight[DEFINE_MAX_BONE_LINK_SIZE]{};
	
		BoneIDWeights();
	
		static void T_get_input_layout(std::vector<D3D11_INPUT_ELEMENT_DESC>& Input_element_desc);
		void normalize();
	};
}

namespace MESH
{
	class SubstanceMesh : public Mesh
	{
	public:
		// resource
		std::vector<BTYPE::Vertex_default>		vertex;
		std::vector<BTYPE::Substance>			substance;
		std::vector<BTYPE::BoneIDWeights>		bone_id_weights;
		std::vector<BTYPE::IB_Default>			index;
		// result
		VertexBuffer<BTYPE::Vertex_default>		buffer_vertex;
		VertexBuffer<BTYPE::Substance>			buffer_substance;
		VertexBuffer<BTYPE::BoneIDWeights>		buffer_bone_id_weights;
		IndexBuffer<BTYPE::IB_Default>			buffer_index;

		SafePtr<SKELETON::Skeleton>				default_skeleton;
		SafePtr<SHADER::MeshShader>				default_shader;

		virtual ~SubstanceMesh();
		virtual std::type_index get_class_type() override;

		virtual void load_resource(Engine* engine) override;
		virtual void release_resource(Engine* engine) override;

		void set_substance_mesh(SubGraphics* sub_graphics, UINT offset = 0);
		void draw_substance_mesh(SubGraphics* sub_graphics);

		virtual void draw_detail_view() override;
	};
	//REGIST_RESOURCE(SubstanceMesh);
}