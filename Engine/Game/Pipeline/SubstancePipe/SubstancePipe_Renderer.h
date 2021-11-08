#pragma once
#include "SubstancePipe.h"

class BoneInstanceStruct
{
public:
	UINT parent_index;
	SafePtr<GameObject> game_object;

	BoneInstanceStruct();
	BoneInstanceStruct(UINT parent_index, SafePtr<GameObject> game_object);
};

// Component로 옮기고 싶은데.
// bone의 경우 object에 있는게 더 합리적인가?
class BoneRoot : public Component
{
public:
	SafePtr<SKELETON::Skeleton>	skeleton;
	std::vector<BoneInstanceStruct> bone_structs;
	ConstantArrayBuffer<BTYPE::CB_World> bone_buffer;
	
	BoneRoot();

	virtual void create_bone();
	void set_bone_transform(SubGraphics* sub_graphics);

	virtual void draw_detail_view() override;
};
REGIST_COMPONENT(BoneRoot);

class BoneChild : public BoneRoot
{
public:
	SafePtr<BoneRoot> bone_root;
	virtual void create_bone() override;
	virtual void draw_detail_view() override;
};
REGIST_COMPONENT(BoneChild);

class SubstancePipe
{
public:
	SafePtr<BoneRoot> bone;
	SafePtr<SHADER::MeshShader> shader;
	SafePtr<MESH::SubstanceMesh> mesh;

	bool is_draw_able();
	void draw(SubGraphics* sub_graphics);
	void draw_mesh_only(SubGraphics* sub_graphics);
	void draw_substance_pipe_detail();
};

class SubstanceRenderer : public ViaRenderer
{
public:
	SubstancePipe Substance_pipe;
	SubstanceRenderer();
	SubstanceRenderer(SafePtr<MESH::SubstanceMesh> substance_mesh);
	virtual std::type_index get_class_type() override;

	virtual void draw(SubGraphics* sub_graphics) override;
	virtual void draw_mesh_only(SubGraphics* sub_graphics) override;
	virtual void draw_detail_view() override;
};
REGIST_COMPONENT(SubstanceRenderer);