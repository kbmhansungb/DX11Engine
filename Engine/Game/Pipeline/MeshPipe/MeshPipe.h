#pragma once
#include "../../../Game/_Exlude.h"
#include "../../../Component/Renderer/_ViaRenderer.h"
#include "../SpritePipe/SpritePipe.h"

namespace MESH
{
	class MeshMesh : public Mesh
	{
	public:
		struct Raw_MeshMesh
		{
			std::string									source_path;
			UINT										mesh_index;
		};
		struct Product_MeshMesh
		{
			std::vector<BTYPE::Vertex_default>			vertex;
			std::vector<BTYPE::IB_Default>				index;

			VertexBuffer<BTYPE::Vertex_default>			buffer_vertex;
			IndexBuffer<BTYPE::IB_Default>				buffer_index;
		};
		struct Addition_MeshMesh
		{
			SafePtr<SHADER::MeshShader>					default_shader;
		};
		Raw_MeshMesh		raw_mm;
		Product_MeshMesh	product_mm;
		Addition_MeshMesh	addition_mm;

		virtual ~MeshMesh();
		virtual std::type_index get_class_type() override;

		virtual void load_resource(Engine* engine) override;
		virtual void release_resource(Engine* engine) override;

		void set_mesh_mesh(SubGraphics* sub_graphics, UINT offset = 0);
		void draw_mesh_mesh(SubGraphics* sub_graphics);

		virtual void draw_detail_view() override;
	};
	REGIST_RESOURCE(MeshMesh);
}

class MeshPipe
{
public:
	SafePtr<SHADER::MeshShader>		shader;
	SafePtr<MESH::MeshMesh>			mesh;

	bool is_draw_able()
	{
		if (shader.is_vaild() == false) return false;
		if (mesh.is_vaild() == false) return false;
		return true;
	}
	void draw(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world);
	void draw_mesh_only(SubGraphics* sub_graphics, ConstantBuffer<BTYPE::CB_World>& world);
	void mesh_pipe_draw_detail();
};

class MeshRenderer : public ViaRenderer
{
public:
	virtual ~MeshRenderer() {};
	// Component을(를) 통해 상속됨
	virtual std::type_index get_class_type() override;

	MeshPipe mesh_pipe;

	virtual void draw(SubGraphics* sub_graphics) override;
	virtual void draw_mesh_only(SubGraphics* sub_graphics) override;
	virtual void draw_detail_view() override;
};
REGIST_COMPONENT(MeshRenderer);

class MeshRendererEditorWindow : public EditorDetailWindow
{
public:
	SafePtr<MeshRenderer> mesh_renderer;
	MeshRendererEditorWindow(SafePtr<MeshRenderer> mesh_renderer);
	virtual void draw_detail_view() override;
};

class EditorDetailWindow_ImageView;
class GameObject;
class RenderToDeferred;
class Camera;
class Graphics;
namespace EDITOR
{
	struct Editor_add_mesh
	{
		SafePtr<GameObject> mesh_object;
		SafePtr<MeshRenderer> mesh_renderer;
		static Editor_add_mesh* get_instance();
	};
	typedef Editor_add_mesh EAM;
	extern void editor_add_mesh(SafePtr<GameObject> parent, SafePtr<MESH::MeshMesh> mesh_mesh);
	struct Free_view_mesh_mesh_renderer
	{
		SafePtr<EditorDetailComponent>		editor_detail_component;

		static Free_view_mesh_mesh_renderer* get_instance();
	};
	typedef Free_view_mesh_mesh_renderer FVMMR;
	extern void free_view_mesh_mesh_renderer(SafePtr<MESH::MeshMesh> mesh_mesh);
}