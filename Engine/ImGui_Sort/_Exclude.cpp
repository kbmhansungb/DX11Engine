#include "_Exclude.h"
#include "../Window/StringHelper.h"

bool ImGui::RightAngleButton(const char* ch, float padding)
{
	ImGui::SameLine(
		ImGui::GetWindowContentRegionMax().x
		- ImGui::CalcTextSize(ch, NULL, true).x
		- padding
	);
	return ImGui::Button(ch);
}

void ImGui::RightAngle(float padding)
{
	ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - padding);
}

bool ImGui::sizeable_check_box(bool* bool_ptr, const char* char_ptr, float offset, float fontsize)
{
	
	ImGuiContext& context = *ImGui::GetCurrentContext();
	ImGuiWindow& window = *ImGui::GetCurrentWindow();
	float backup = context.FontSize;
	context.FontSize = fontsize;
	window.DC.CursorPos.x += offset;
	window.DC.CursorPos.y += offset;

	// 선택 버튼에 대해서 정의
	bool return_val = ImGui::Checkbox(char_ptr, bool_ptr);
	ImGui::PushItemWidth(0);

	//window.DC.CursorPos.x -= offset;
	window.DC.CursorPos.y -= offset;
	context.FontSize = backup;

	return return_val;
}

void ImGui::set_font_size(float re_font_size, float* backup)
{
	ImGuiContext& context = *ImGui::GetCurrentContext();
	if (backup != nullptr)
	{
		*backup = context.FontSize;
	}
	context.FontSize = re_font_size;
}

void ImGui::wstring_field(std::wstring& wstr, std::string name, size_t buff_size)
{
	std::string str = StringHelper::WideToString(wstr);
	ImGui::Text_field(str, name, buff_size);
	wstr = StringHelper::StringToWide(str);
}

bool ImGui::INT_field(int* ptr, std::string name)
{
	std::string id = name + "##" + StringHelper::ptr_to_string(&ptr);
	return ImGui::InputInt(id.c_str(), ptr);
}

bool ImGui::INT2_field(int* ptr, std::string name)
{
	std::string id = name + "##" + StringHelper::ptr_to_string(&ptr);
	return ImGui::InputInt2(id.c_str(), ptr);
}

bool ImGui::INT3_field(int* ptr, std::string name)
{
	std::string id = name + "##" + StringHelper::ptr_to_string(&ptr);
	return ImGui::InputInt3(id.c_str(), ptr);
}

bool ImGui::float_field(float* ptr, std::string name)
{
	std::string id = name + "##" + StringHelper::ptr_to_string(&ptr);
	return ImGui::InputFloat(id.c_str(), ptr);
}

bool ImGui::XMFLOAT2_field(XMFLOAT2& float3, std::string name)
{
	std::string id = name + "##" + StringHelper::ptr_to_string(&float3);
	return ImGui::InputFloat2(id.c_str(), &float3.x);
}

bool ImGui::XMFLOAT3_field(XMFLOAT3& float3, std::string name)
{
	std::string id = name + "##" + StringHelper::ptr_to_string(&float3);
	return ImGui::InputFloat3(id.c_str(), &float3.x);
}

bool ImGui::XMVECTOR_field(XMVECTOR& vec, std::string name)
{
	std::string id = name + "##" + StringHelper::ptr_to_string(&vec);
	return ImGui::InputFloat4(id.c_str(), &vec.m128_f32[0]);
}

static bool Open_setting_state = false;
static bool Use_matrix = false;
static bool Use_vector = true;
void ImGui::XMMATRIX_field(XMMATRIX& mat, std::string str, std::string key)
{
	//if (ImGui::TreeNode((str + "##" + key).c_str()))
	{
		ImGui::NewLine();
		ImGui::RightAngle(15.0f);
		ImGui::sizeable_check_box(&Open_setting_state, ("##" + key).c_str());
		if (Open_setting_state)
		{
			float backup;
			ImGui::set_font_size(13.0f, &backup);
			ImGui::Checkbox("Use matrix", &Use_matrix);
			ImGui::Checkbox("Use vector", &Use_vector);
			ImGui::set_font_size(backup);
		}

		if (Use_matrix)
		{
			XMMATRIX copy_mat = mat;
			if	(
				ImGui::InputFloat4(("r0##" + std::to_string(copy_mat.r[0].m128_f32[0])).c_str(), &copy_mat.r[0].m128_f32[0]) ||
				ImGui::InputFloat4(("r1##" + std::to_string(copy_mat.r[1].m128_f32[0])).c_str(), &copy_mat.r[1].m128_f32[0]) ||
				ImGui::InputFloat4(("r2##" + std::to_string(copy_mat.r[2].m128_f32[0])).c_str(), &copy_mat.r[2].m128_f32[0]) ||
				ImGui::InputFloat4(("r3##" + std::to_string(copy_mat.r[3].m128_f32[0])).c_str(), &copy_mat.r[3].m128_f32[0])
				)
			{
				mat = copy_mat;
			}
		}
		if (Use_vector)
		{
			XMVECTOR pos;
			XMVECTOR quat;
			XMVECTOR sca;
			XMVECTOR rot;

			XMMatrixDecompose(&sca, &quat, &pos, mat);
			float x = quat.m128_f32[0];
			float y = quat.m128_f32[1];
			float z = quat.m128_f32[2];
			float w = quat.m128_f32[3];
			float sqx = x * x;
			float sqy = y * y;
			float sqz = z * z;
			float sqw = w * w;
			
			// roll
			rot.m128_f32[0] = XMConvertToDegrees(std::asinf(2.0f * (w * x - y * z)));
			// pitch
			rot.m128_f32[1] = XMConvertToDegrees(std::atan2f(2.0f * (x * z + w * y), (-sqx - sqy + sqz + sqw)));
			// yaw
			rot.m128_f32[2] = XMConvertToDegrees(std::atan2f(2.0f * (x * y + w * z), (-sqx + sqy -sqz + sqw)));

			bool result0 = ImGui::InputFloat3(("Position##" + std::to_string(pos.m128_f32[0])).c_str(), &pos.m128_f32[0]);
			bool result1 = ImGui::InputFloat3(("Rotation##" + std::to_string(rot.m128_f32[0])).c_str(), &rot.m128_f32[0]);
			bool result2 = ImGui::InputFloat3(("Scale##" + std::to_string(sca.m128_f32[0])).c_str(), &sca.m128_f32[0]);
			if (result0 || result1 || result2)
			{
				mat = XMMatrixScaling(sca.m128_f32[0], sca.m128_f32[1], sca.m128_f32[2])
					* XMMatrixRotationRollPitchYaw(
						XMConvertToRadians(rot.m128_f32[0]), 
						XMConvertToRadians(rot.m128_f32[1]),
						XMConvertToRadians(rot.m128_f32[2]))
					* XMMatrixTranslation(pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2]);
			}
		}

		//ImGui::TreePop();
	}
}

void ImGui::XMMATRIX_field(XMMATRIX& mat, std::string display)
{
	XMMATRIX_field(mat, display, StringHelper::ptr_to_string(&mat));
}

void ImGui::Text_field(std::string& text, std::string name, size_t buff_size)
{
	static char char_con[1000];
	memset(char_con, 0, 1000);
	strcpy_s(char_con, text.c_str());

	std::string ID = name + "##" + StringHelper::ptr_to_string(&text);

	if (ImGui::InputText(ID.c_str(), char_con, buff_size))
	{
		text = char_con;
	}
}
