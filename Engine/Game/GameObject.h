#pragma once

#include "_Include.h"
#include "Graphics/_Exclude.h" // for constant buffer
#include "Component.h"


namespace TRANSFORM_DEFAULT
{
	static const DirectX::XMVECTOR FORWARD_VECTOR		= DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	static const DirectX::XMVECTOR RIGHT_VECTOR			= DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	static const DirectX::XMVECTOR UP_VECTOR			= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	static const DirectX::XMMATRIX MATRIX				= DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	static const DirectX::XMMATRIX MATRIX_IDENTITY		= DirectX::XMMatrixIdentity();
}

// game objct는 상대 좌표임.
class GameObject final : 
	public Base
{
public:
	//// 일단은 싱글코어를 바탕으로 생각함.
	//// malloc으로 생성한다면 피해갈 수 있지 않을까.
	//struct CS_CreateGameObject final
	//{
	//	struct CSCGOField
	//	{
	//		Scene* scene = nullptr;
	//		GameObject* parent = nullptr;
	//		static CSCGOField* get_instance();
	//	};
	//private:
	//	CS_CreateGameObject() = delete;
	//public:
	//	GameObject* context;

	//	CS_CreateGameObject(GameObject* game_object);
	//	~CS_CreateGameObject();
	//	template<typename ChildClass>
	//	SafePtr<ChildClass> call_constructor(ChildClass* new_game_object, std::string name = "")
	//	{
	//		context->_childs.emplace_back(new_game_object);
	//		context->_childs.back()->name = name;
	//		return context->_childs.back()->get_safe_ptr().cast<ChildClass>();
	//	}
	//};

public:
	GameObject(SafePtr<Scene> scene, std::string name, SafePtr<GameObject> parent);
	GameObject();
	virtual ~GameObject() {};
	virtual std::type_index get_class_type() { return typeid(GameObject); }

#pragma region Val
public:

private:	bool _active = true;
public:		__declspec(property(get = get_active, put = put_active)) bool active;
	  bool get_active() { return _active; }
	  void put_active(bool active) { _active = active; }

	  void update();

private:	SafePtr<Scene> _this_scene;
public:		SafePtr<Scene> get_this_scene() { return _this_scene; }
			__declspec(property(get = get_this_scene)) SafePtr<Scene> this_scene;

private:	SafePtr<GameObject> _parent;
public:		SafePtr<GameObject>	get_parent() { return _parent; }
			__declspec(property(get = get_parent)) SafePtr<GameObject> parent;

private:	std::list<std::unique_ptr<GameObject>> _childs;
public:		std::list<std::unique_ptr<GameObject>>& get_childs() { return _childs; }
			__declspec(property(get = get_childs)) std::list<std::unique_ptr<GameObject>> childs;
		//CS_CreateGameObject add_child();
		SafePtr<GameObject> add_child(std::string child_object_name);
		void remove_child(SafePtr<GameObject> game_object);
		SafePtr<GameObject> find_child_by_name(std::string target_name);

private:	std::list<std::unique_ptr<Component>> _components;
public:		std::list<std::unique_ptr<Component>>& get_components() { return _components; }
			__declspec(property(get = get_components)) std::list<std::unique_ptr<Component>> components;
	template <class ComponentType>
	SafePtr<ComponentType> add_component(ComponentType* new_component)
	{
		_components.emplace_back(new_component);

		SafePtr<ComponentType> result = _components.back()->get_safe_ptr().cast<ComponentType>();
		result->_owner = this;

		//result->initialize();

		return result;
	}
	template <class ComponentType>
	SafePtr<ComponentType> add_component()
	{
		_components.emplace_back(new ComponentType());

		SafePtr<ComponentType> result 
			= _components.back()->get_safe_ptr()
			.cast<ComponentType>();
		result->_owner = this;
		return result;
	}
	template <class Type>
	SafePtr<Type> get_component()
	{
		for (auto& iter : _components)
		{
			Type* cast_result = dynamic_cast<Type*>(iter.get());
			if (cast_result != nullptr) return iter->get_safe_ptr().cast<Type>();
		}
		return SafePtr<Type>();
	}

	std::string name;

	//////
	// Transform

	ConstantBuffer<BTYPE::CB_World> local;
	ConstantBuffer<BTYPE::CB_World> world;

	Delegate::delegate<SafePtr<GameObject>> Delegate_update_matrix;
	void UpdateMatrix();

	// Transform	
	XMVECTOR pos;
	XMVECTOR rot;
	XMVECTOR scale;

	XMVECTOR vecForward;
	XMVECTOR vecRight;
	XMVECTOR vecUp;

	//Transform
	const DirectX::XMVECTOR& GetPosition() const;
	const DirectX::XMVECTOR& GetRotation() const;
	const DirectX::XMVECTOR& GetScale() const;

	const DirectX::XMVECTOR& GetForwardVector();
	const DirectX::XMVECTOR& GetRightVector();
	const DirectX::XMVECTOR& GetUpVecctor();

private:
	struct ClassStack_AdjustTransform final
	{
	private:
		//Transform<UseClass>* transform;
		GameObject* transform;
		ClassStack_AdjustTransform() = delete;
	public:
		ClassStack_AdjustTransform(GameObject* transform);
		~ClassStack_AdjustTransform();

		ClassStack_AdjustTransform* set_local_transform(const XMMATRIX& matrix);
		ClassStack_AdjustTransform* SetPosition(float x, float y, float z);
		ClassStack_AdjustTransform* AdjustPosition(float x, float y, float z);
		ClassStack_AdjustTransform* SetRotation(float x, float y, float z);
		ClassStack_AdjustTransform* AdjustRotation(float x, float y, float z);
		ClassStack_AdjustTransform* SetScale(float x, float y, float z);
		ClassStack_AdjustTransform* AdjustScale(float x, float y, float z);
		ClassStack_AdjustTransform* SetLookAtPos(DirectX::XMFLOAT3 lookAt);
	};
public:
	ClassStack_AdjustTransform Adjust();

#pragma endregion

	friend Scene;

	virtual void draw_detail_view() override;
};

class EditorDetailWindow : public Base
{
public:
	virtual std::type_index get_class_type();
	virtual void detail_window_detail_view() = 0;

	bool is_selected_window = false;
	Delegate::delegate<UINT, UINT> delegate_view_size_is_changed;

	void update_is_selected_window();
	ImVec2 last_window_pos;
	float title_bar_height;
	float menu_bar_height;
	ImVec2 get_relative_mouse_position();

	//void invoke_controll(WindowInputContainer* wic, bool add = true);
	//void redirect_mouse_event(MouseEvent me);
	//Delegate::delegate<MouseEvent> delegate_mouse_event;
	//void redirect_keyboard_event(KeyboardEvent ke);
	//Delegate::delegate<KeyboardEvent> delegate_keyboard_event;
	//void redirect_cha_event(unsigned char ch);
	//Delegate::delegate<unsigned char> delegate_cha_event;
};
class EditorDetailComponent : public Component
{
public:
	EditorDetailComponent(EditorDetailWindow* editor_detail_window_ptr);
	virtual std::type_index get_class_type();

private:
	bool _is_draw_detail_window = false;
public:
	bool get_is_draw_detail_window();
	void set_is_draw_detail_window(bool new_is_draw_detail_window);
	__declspec(property(get = get_is_draw_detail_window, put = set_is_draw_detail_window))
		bool is_draw_detail_window;
	std::unique_ptr<EditorDetailWindow> editor_detail_window;
	virtual void draw_detail_view() override;
};