#include "GameObject.h"
#include "Scene.h"
#include "../Game/Engine.h"

#include "../Template/EditorWindow/EditorWindow.h"

// Root game object 생성용

using namespace std;

//unique_ptr<GameObject::CS_CreateGameObject::CSCGOField> unique_class_stack_create_game_object_field;
//GameObject::CS_CreateGameObject::CSCGOField* GameObject::CS_CreateGameObject::CSCGOField::get_instance()
//{
//	if (unique_class_stack_create_game_object_field.get() == nullptr)
//		unique_class_stack_create_game_object_field 
//		= make_unique<GameObject::CS_CreateGameObject::CSCGOField>();
//	return unique_class_stack_create_game_object_field.get();
//}
//
//GameObject::CS_CreateGameObject::CS_CreateGameObject(GameObject* game_object) :
//	context(game_object)
//{
//	CSCGOField* field = CSCGOField::get_instance();
//	field->scene = game_object->_this_scene.get();
//	field->parent = game_object;
//}
//
//GameObject::CS_CreateGameObject::~CS_CreateGameObject()
//{
//	CSCGOField* field = CSCGOField::get_instance();
//	field->scene = nullptr;
//	field->parent = nullptr;
//}

GameObject::GameObject(SafePtr<Scene> scene, string name, SafePtr<GameObject> parent) :
	_this_scene(scene),
	name(name),
	_parent(parent)
{
	//CS_CreateGameObject::CSCGOField* field
	//	= CS_CreateGameObject::CSCGOField::get_instance();

	//if (field->scene != nullptr)
	//{
	//	this->_this_scene = field->scene->get_safe_ptr().cast<Scene>();
	//}

	//if (field->parent != nullptr)
	//{
	//	this->_parent = field->scene->get_safe_ptr().cast<GameObject>();
	//}

	ID3D11Device* device = this->_this_scene->this_engine->Device.Get();
	local.initialize(device);
	world.initialize(device);

	Adjust()
		.SetPosition(0.0f, 0.0f, 0.0f)
		->SetRotation(0.0f, 0.0f, 0.0f)
		->SetScale(1.0f, 1.0f, 1.0f);
}

GameObject::GameObject()
{
	//CS_CreateGameObject::CSCGOField* field
	//	= CS_CreateGameObject::CSCGOField::get_instance();

	//if (field->scene != nullptr)
	//{
	//	this->_this_scene = field->scene->get_safe_ptr().cast<Scene>();
	//}

	//if (field->parent != nullptr) 
	//{
	//	this->_parent = field->parent->get_safe_ptr().cast<GameObject>();
	//}

	if (this->_this_scene.is_vaild() == false) return;

	ID3D11Device* device = this->_this_scene->this_engine->Device.Get();
	local.initialize(device);
	world.initialize(device);

	Adjust()
		.SetPosition(0.0f, 0.0f, 0.0f)
		->SetRotation(0.0f, 0.0f, 0.0f)
		->SetScale(1.0f, 1.0f, 1.0f);
}

void GameObject::update()
{
	for (auto& child_object : this->_childs)
	{
		if (child_object->active) child_object->update();
	}

	for (auto& component : this->_components)
	{
		if (component->active) component->update();
	}
}

SafePtr<GameObject> GameObject::add_child(std::string child_object_name)
{
	return _childs.emplace_back(new GameObject(_this_scene, child_object_name, this)).get()->get_safe_ptr().cast<GameObject>();
}

void GameObject::remove_child(SafePtr<GameObject> game_object)
{
	GameObject* game_object_ptr = game_object.get();
	auto find_result = find_if(_childs.begin(), _childs.end(), [game_object_ptr](unique_ptr<GameObject>& child)
		{
			return child.get() == game_object_ptr;
		});
	if (find_result != _childs.end())
		_childs.erase(find_result);
}

void find_child_by_name_search_hierarchy(
	SafePtr<GameObject> start,
	const std::string& target_name,
	SafePtr<GameObject>& result)
{
	if (start->name._Equal(target_name))
		result = start;
	else
	{
		for (auto& child : start->childs)
		{
			find_child_by_name_search_hierarchy(
				child->get_safe_ptr().cast<GameObject>(),
				target_name,
				result
			);
			if (result.is_vaild()) return;
		}
	}
}
SafePtr<GameObject> GameObject::find_child_by_name(std::string target_name)
{
	SafePtr<GameObject> search_result;
	find_child_by_name_search_hierarchy(
		this->get_safe_ptr().cast<GameObject>(),
		target_name,
		search_result
	);
	return search_result;
}

//Transform
void GameObject::UpdateMatrix()
{
	ID3D11DeviceContext* device_context = this->_this_scene->this_engine->Device_context.Get();

	// 로컬 메트릭스 업데이트
	this->local.data.world
		= DirectX::XMMatrixScaling(this->scale.m128_f32[0], this->scale.m128_f32[1], this->scale.m128_f32[2])
		* DirectX::XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(this->rot.m128_f32[0]), 
			XMConvertToRadians(this->rot.m128_f32[1]), 
			XMConvertToRadians(this->rot.m128_f32[2])
		)
		* DirectX::XMMatrixTranslation(this->pos.m128_f32[0], this->pos.m128_f32[1], this->pos.m128_f32[2]);
	local.apply_changes(device_context);

	// 월드 메트릭스 업데이트
	XMMATRIX parent_matrix = TRANSFORM_DEFAULT::MATRIX_IDENTITY;
	if (this->_parent.is_vaild())
	{
		parent_matrix = _parent->world.data.world;
	}
	world.data.world = local.data.world * parent_matrix;
	world.apply_changes(device_context);

	// 로컬 벡터 업데이트
	XMVECTOR s, r, t;
	XMMatrixDecompose(&s, &r, &t, world.data.world);
	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationQuaternion(r);
	this->vecForward = DirectX::XMVector3TransformCoord(TRANSFORM_DEFAULT::FORWARD_VECTOR, rotMat);
	this->vecRight = DirectX::XMVector3TransformCoord(TRANSFORM_DEFAULT::RIGHT_VECTOR, rotMat);
	this->vecUp = DirectX::XMVector3TransformCoord(TRANSFORM_DEFAULT::UP_VECTOR, rotMat);

	// 업데이트된 메트릭스 전파
	this->Delegate_update_matrix.Invoke(this);

	for (auto& child : _childs)
	{
		child->UpdateMatrix();
	}
}

const DirectX::XMVECTOR& GameObject::GetPosition() const
{
	return this->pos;
}

const DirectX::XMVECTOR& GameObject::GetRotation() const
{
	return this->rot;
}

const DirectX::XMVECTOR& GameObject::GetScale() const
{
	return scale;
}

const DirectX::XMVECTOR& GameObject::GetForwardVector()
{
	return this->vecForward;
}

const DirectX::XMVECTOR& GameObject::GetRightVector()
{
	return this->vecRight;
}

const DirectX::XMVECTOR& GameObject::GetUpVecctor()
{
	return this->vecUp;
}

GameObject::ClassStack_AdjustTransform GameObject::Adjust()
{
	return ClassStack_AdjustTransform(this);
}

void GameObject::draw_detail_view()
{
	Base::draw_detail_view();

	if (ImGui::CollapsingHeader("Game object"))
	{
		if (this->_parent.is_vaild())
			ImGui::Text_field(this->name, "Name", 100);

		XMFLOAT3 v1 = { pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2] };
		XMFLOAT3 v2 = { rot.m128_f32[0], rot.m128_f32[1], rot.m128_f32[2] };
		XMFLOAT3 v3 = { scale.m128_f32[0], scale.m128_f32[1], scale.m128_f32[2] };

		if (ImGui::XMFLOAT3_field(v1, "Position"))
		{
			Adjust().SetPosition(v1.x, v1.y, v1.z);
		}
		if (ImGui::XMFLOAT3_field(v2, "Rotation"))
		{
			Adjust().SetRotation(v2.x, v2.y, v2.z);
		}
		if (ImGui::XMFLOAT3_field(v3, "Scale"))
		{
			Adjust().SetScale(v3.x, v3.y, v3.z);
		}

		ImGui::NewLine();
		if (ImGui::RightAngleButton("Delete"))
		{
			this->_parent->remove_child(this);
			return;
		}
		ImGui::NewLine();
		if (ImGui::RightAngleButton("Add child"))
		{
			this->add_child("Child game object");
		}

		if (ImGui::TreeNode("Component"))
		{
			// add component popup
			static string text_field = "";
			if (ImGui::BeginPopup("Add component popup"))
			{
				ImGui::Text_field(text_field, "Type", 100);

				auto& resource_map = EDITOR::Define::get_define()->component_construct_map;
				for (auto iter = resource_map.begin(); iter != resource_map.end(); ++iter)
				{
					if (iter->first.find(text_field) != string::npos)
					{
						// 해당하는 스트링이 있는 경우에만.
						if (ImGui::Button(iter->first.c_str(), ImVec2(-1, 0)))
						{
							// 버튼이 눌리면 컴포넌트 추가
							iter->second->add_component_to(this);
						}
					}
				}

				ImGui::EndPopup();
			}

			// Add component
			if (ImGui::RightAngleButton("Add Component")) ImGui::OpenPopup("Add component popup");

			// 존재하는 컴포넌트 나열
			auto delete_target = _components.end();
			for (auto iter = _components.begin(); iter != _components.end(); iter++)
			{
				Component* component = iter->get();

				type_index type = typeid(*component);
				string name 
					= std::string(type.name()) 
					+ "##" 
					+ StringHelper::ptr_to_string(component);
				if (ImGui::TreeNode(name.c_str()))
				{
					string buttom_name
						= "Delete##"
						+ StringHelper::ptr_to_string(component);
					if (ImGui::RightAngleButton(buttom_name.c_str()))
					{
						delete_target = iter;
					}

					component->draw_detail_view();
					ImGui::TreePop();
				}
			}

			if (delete_target != _components.end())
			{
				_components.erase(delete_target);
			}
			ImGui::TreePop();
		}
	}
}

GameObject::ClassStack_AdjustTransform::ClassStack_AdjustTransform(GameObject* transform) : transform(transform) {}

GameObject::ClassStack_AdjustTransform::~ClassStack_AdjustTransform()
{
	transform->UpdateMatrix();
}
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: QuatToPitchYawRoll")
// https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=spinx85&logNo=140120555548
// 를 참고함.
// Quat to radian
void QuatToPitchYawRoll(
	const XMVECTOR& q,
	float& pitch,
	float& yaw,
	float& roll
)
{
	float 
		x = q.m128_f32[0], 
		y = q.m128_f32[1], 
		z = q.m128_f32[2], 
		w = q.m128_f32[3];
	float sqw = w * w;
	float sqx = x * x;
	float sqy = y * y;
	float sqz = z * z;

	// rotation about x-axis
	pitch = asinf(2.0f * (w * x - y * z));
	// rotation about y-axis
	yaw = atan2f(2.0f * (x * z + w * y), (-sqx - sqy + sqz + sqw));
	// rotation about z-axis
	roll = atan2f(2.0f * (x * y + w * z), (-sqx + sqy - sqz + sqw));
}
GameObject::ClassStack_AdjustTransform* GameObject::ClassStack_AdjustTransform::set_local_transform(const XMMATRIX& matrix)
{
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: rot의 경우 쿼터에서 오일러로 변환해서 저장해야함.")
	XMVECTOR out_pos;
	XMVECTOR out_quat;
	XMVECTOR out_scale;
	XMMatrixDecompose(&out_scale, &out_quat, &out_pos, matrix);
	
#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 이거 왜 이렇게 되는지 모름")
	float pitch, yaw, roll;
	QuatToPitchYawRoll(out_quat, pitch, yaw, roll);
	pitch = XMConvertToDegrees(pitch);
	yaw = XMConvertToDegrees(yaw);
	roll = XMConvertToDegrees(roll);

	this->SetPosition(out_pos.m128_f32[0], out_pos.m128_f32[1], out_pos.m128_f32[2]);
	this->SetRotation(pitch, yaw, roll);
	this->SetScale(out_scale.m128_f32[0], out_scale.m128_f32[1], out_scale.m128_f32[2]);

	//ImGui::XMMATRIX_field

	return this;
}

GameObject::ClassStack_AdjustTransform* GameObject::ClassStack_AdjustTransform::SetPosition(float x, float y, float z)
{
	transform->pos = { x, y, z, 0.f };
	return this;
}

GameObject::ClassStack_AdjustTransform* GameObject::ClassStack_AdjustTransform::AdjustPosition(float x, float y, float z)
{
	transform->pos.m128_f32[0] += x;
	transform->pos.m128_f32[1] += y;
	transform->pos.m128_f32[2] += z;
	return this;
}

GameObject::ClassStack_AdjustTransform* GameObject::ClassStack_AdjustTransform::SetRotation(float x, float y, float z)
{
	transform->rot = { x, y, z, 0.0f };
	return this;
}


GameObject::ClassStack_AdjustTransform* GameObject::ClassStack_AdjustTransform::AdjustRotation(float x, float y, float z)
{
	transform->rot.m128_f32[0] += x;
	transform->rot.m128_f32[1] += y;
	transform->rot.m128_f32[2] += z;
	return this;
}

GameObject::ClassStack_AdjustTransform* GameObject::ClassStack_AdjustTransform::SetScale(float x, float y, float z)
{
	transform->scale = { x, y, z, 0.0f };
	return this;
}

GameObject::ClassStack_AdjustTransform* GameObject::ClassStack_AdjustTransform::AdjustScale(float x, float y, float z)
{
	transform->scale.m128_f32[0] += x;
	transform->scale.m128_f32[1] += y;
	transform->scale.m128_f32[2] += z;
	return this;
}

GameObject::ClassStack_AdjustTransform* GameObject::ClassStack_AdjustTransform::SetLookAtPos(DirectX::XMFLOAT3 lookAt)
{
	lookAt.x = transform->pos.m128_f32[0] - lookAt.x;
	lookAt.y = transform->pos.m128_f32[1] - lookAt.y;
	lookAt.z = transform->pos.m128_f32[2] - lookAt.z;

	float pitch = 0.0f;
	if (lookAt.y != 0.0f)
	{
		const float dist = sqrtf(lookAt.x * lookAt.x + lookAt.z * lookAt.z);
		if (dist == 0.0f)
		{
			return this;
		}
		pitch = atan(lookAt.y / dist);
	}

	float yaw = 0.0f;
	if (lookAt.x != 0.0f)
	{
		if (lookAt.z == 0.0f)
		{
			return this;
		}
		yaw = atan(lookAt.x / lookAt.z);
	}
	if (lookAt.z > 0)
	{
		yaw += DirectX::XM_PI;
	}
	return this;
}

std::type_index EditorDetailWindow::get_class_type() { return typeid(EditorDetailWindow); }

void EditorDetailWindow::update_is_selected_window()
{
	auto aw = ImGui::GetCurrentContext()->ActiveIdWindow;
	auto cw = ImGui::GetCurrentWindow();
	
	if (aw != nullptr) this->is_selected_window = aw == cw;
	last_window_pos = cw->Pos;
	title_bar_height = cw->TitleBarHeight();
	menu_bar_height = cw->MenuBarHeight();
	//if (is_selected_window)
	//{
	//	int a = 3;
	//}
}

ImVec2 EditorDetailWindow::get_relative_mouse_position()
{
	auto mpos = ImGui::GetIO().MousePos;
	auto rmpos = ImVec2(
		mpos.x - last_window_pos.x,
		mpos.y - last_window_pos.y
		- title_bar_height
		- menu_bar_height
	);
	return rmpos;
}

//void EditorDetailWindow::invoke_controll(WindowInputContainer* wic, bool add)
//{
//	if (add)
//	{
//		wic->Delegate_mouse_event
//			.AddInvoker(this, &EditorDetailWindow::redirect_mouse_event);
//		wic->Delegate_keyboard_event
//			.AddInvoker(this, &EditorDetailWindow::redirect_keyboard_event);
//		wic->Delegate_keyborad_cha
//			.AddInvoker(this, &EditorDetailWindow::redirect_cha_event);
//	}
//	else
//	{
//		wic->Delegate_mouse_event.RemoveNode(this);
//		wic->Delegate_keyboard_event.RemoveNode(this);
//		wic->Delegate_keyborad_cha.RemoveNode(this);
//	}
//}
//
//void EditorDetailWindow::redirect_mouse_event(MouseEvent me)
//{
//	if (is_selected_window)
//	{
//		this->delegate_mouse_event.Invoke(me);
//
//		switch (me.GetType())
//		{
//		case MouseEventType::LPRESS:
//		{
//			int a = 3;
//		}
//			break;
//		}
//	}
//}
//
//void EditorDetailWindow::redirect_keyboard_event(KeyboardEvent ke)
//{
//	if (is_selected_window)
//	{
//		this->delegate_keyboard_event.Invoke(ke);
//	}
//}
//
//void EditorDetailWindow::redirect_cha_event(unsigned char ch)
//{
//	if (is_selected_window)
//	{
//		this->delegate_cha_event.Invoke(ch);
//	}
//}

EditorDetailComponent::EditorDetailComponent(EditorDetailWindow* editor_detail_window_ptr)
	: editor_detail_window(editor_detail_window_ptr)
{
	//auto shared = EDITOR::Shared::get_shared();
	//shared->editor_window->ImGui_graphics->ImGui_draw_list.push_back(editor_detail_window_ptr);
	// 생성시 자동 할당은 사용하지 않는다.
}

std::type_index EditorDetailComponent::get_class_type() { return typeid(EditorDetailComponent); }

bool EditorDetailComponent::get_is_draw_detail_window() { return _is_draw_detail_window; }

void EditorDetailComponent::set_is_draw_detail_window(bool new_is_draw_detail_window)
{
	if (new_is_draw_detail_window != _is_draw_detail_window)
	{
		_is_draw_detail_window = new_is_draw_detail_window;
	}
	else
		return;

	auto& I = *EDITOR::CEW::get_instance();
	if (_is_draw_detail_window && this->editor_detail_window.get() != nullptr)
	{
		I.ImGui_graphics->ImGui_draw_list.push_back
		(this->editor_detail_window->get_safe_ptr().cast<Base>());
	}
	else
	{
		I.ImGui_graphics->ImGui_draw_list.remove
		(this->editor_detail_window->get_safe_ptr().cast<Base>());
	}
}

void EditorDetailComponent::draw_detail_view()
{
	Component::draw_detail_view();

	if (ImGui::CollapsingHeader("EditorDetailComponent"))
	{
		if (editor_detail_window.get() != nullptr)
			editor_detail_window->detail_window_detail_view();
	}
}
