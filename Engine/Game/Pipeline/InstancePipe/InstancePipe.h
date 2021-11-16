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

/// <summary>
/// 인스턴스 메쉬 렌더
/// 메쉬를 바탕으로 인스턴스 렌더함.
/// 21.11.16
/// </summary>
class InstanceMeshRenderer : public ViaRenderer
{

#pragma region Val

public:
	InstancePipe instance_pipe;

#pragma endregion

#pragma region ViaRenderer

public:
	virtual void draw(SubGraphics* sub_graphics) override;
	virtual void draw_mesh_only(SubGraphics* sub_graphics) override;
	virtual void draw_detail_view() override;

#pragma endregion

#pragma region Component

public:
	virtual void awake() override;

#pragma endregion

};
REGIST_COMPONENT(InstanceMeshRenderer);