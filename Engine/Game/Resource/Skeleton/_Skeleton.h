#pragma once
#include "../Resource.h"

namespace SKELETON
{
	class Skeleton : public Resource
	{
	public:
		class Bone
		{
		public:
			UINT Bone_index = 0;
			std::string Bone_name = "root_bone";
			UINT Parent_bone = 0;
			//vector<UINT> Child_bones;

			//XMMATRIX Offset_transform = XMMatrixIdentity();
			XMMATRIX Local_transform = XMMatrixIdentity();
			XMMATRIX World_transform = XMMatrixIdentity();
		};

		virtual ~Skeleton();
		virtual void load_resource(Engine* engine) override;
		virtual void release_resource(Engine* engine) override;

		std::vector<Bone> Bones;
		std::unordered_map<std::string, size_t> Name_bone_map;

		Bone* add_new_bone(std::string Bone_name, UINT Bone_index, const XMMATRIX& Offset_transform);
		Bone* make_world(UINT Bone_index, UINT Bone_parent);
		virtual void draw_detail_view() override;
	};
}