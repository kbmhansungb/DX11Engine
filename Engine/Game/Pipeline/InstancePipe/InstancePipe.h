#pragma once
#include "../MeshPipe/_Exclude.h"

/// <summary>
/// Instance pipe는 Mesh pipe의 연장선상에 있다.
/// </summary>
class InstancePipe
{
public:
	SafePtr<SHADER::MeshShader>	shader;
	SafePtr<MESH::MeshMesh>	mesh;
	
	//ConstantBuffer<BTYPE::CB_InstanceSetting>	setting;
	/// <summary>
	/// Instance world buffer is in here.
	/// </summary>
	ConstantArrayBuffer<BTYPE::CB_World_only>	worlds;

	void initialize(Engine* engine);
	bool is_draw_able()
	{
		if (shader.is_vaild() == false) return false;
		if (mesh.is_vaild() == false) return false;
		if (worlds.Get() == nullptr) return false;
		return true;
	}
	void draw(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world);
	void draw_mesh_only(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world);
	void update();

	void draw_instance_detail_view();
};

class InstanceMeshRenderer : public ViaRenderer
{
public:
	InstancePipe instance_pipe;

	// ViaRenderer을(를) 통해 상속됨
	virtual std::type_index get_class_type() override;
	virtual void draw(SubGraphics* sub_graphics) override;
	virtual void draw_mesh_only(SubGraphics* sub_graphics) override;
	virtual void draw_detail_view() override;
	virtual void awake() override;
};
REGIST_COMPONENT(InstanceMeshRenderer);