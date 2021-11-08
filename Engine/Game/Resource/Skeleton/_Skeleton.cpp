#include "_Skeleton.h"

using namespace std;

SKELETON::Skeleton::~Skeleton() {}

void SKELETON::Skeleton::load_resource(Engine* engine) {}

void SKELETON::Skeleton::release_resource(Engine* engine) {}

SKELETON::Skeleton::Bone* SKELETON::Skeleton::add_new_bone(string Bone_name, UINT Bone_index, const XMMATRIX& Offset_transform)
{
	Bones.emplace_back();
	Bones.back().Bone_index = Bone_index;
	Bones.back().Bone_name = Bone_name;
	//Bones.back().Offset_transform = Offset_transform;
	Bones.back().World_transform = Offset_transform;
	//Bones.back().Local_transform = Offset_transform;
	Name_bone_map[Bone_name] = Bones.size() - 1;
	// ��¥ �⺻�� ���ؼ� �𸣴� ����. ������ �迭�� ������Ű��ʳ� ���θ��㶧 ���� �޸𸮸� �����Ѵ�.
	// ������ PTR���� ������ �ִ°� �ƴ϶� UINT���� ������ �ִ� �����̴�.
	// �θ��� �Ѱ��� ������ ������ ó���ȴ�.
	return &Bones[Bones.size() - 1];
}

SKELETON::Skeleton::Bone* SKELETON::Skeleton::make_world(UINT Bone_index, UINT Bone_parent)
{
	// 0, 0�� ��� Identity * Identity��.
	Bones[Bone_index].Parent_bone = Bone_parent;
	if (Bone_index != 0)
		Bones[Bone_index].Local_transform = Bones[Bone_index].World_transform * DirectX::XMMatrixInverse(nullptr, Bones[Bone_parent].World_transform);
	return &Bones[Bone_index];
}

void SKELETON::Skeleton::draw_detail_view()
{
	Resource::draw_detail_view();

	if (ImGui::CollapsingHeader("Skeleton"))
	{
		if (ImGui::TreeNode("Bone data"))
		{
			// ��ȯ�ϸ鼭 ���� �׸���.
			for (auto& bone : Bones)
				if (ImGui::TreeNode(bone.Bone_name.c_str()))
				{
					ImGui::Text(("Bone index : " + to_string(bone.Bone_index)).c_str());
					ImGui::Text(("Parent bone index : " + to_string(bone.Parent_bone)).c_str());

					ImGui::XMMATRIX_field(bone.Local_transform, "Local transform", StringHelper::ptr_to_string(&bone.Local_transform));
					ImGui::XMMATRIX_field(bone.World_transform, "World transform", StringHelper::ptr_to_string(&bone.World_transform));

					ImGui::TreePop();
				}

			ImGui::TreePop();
		}
	}
}
