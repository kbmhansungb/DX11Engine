//#include "SkinShader.h"
//#include "../../Engine.h"
//
//SHADER::SkinShader::SkinShader(wstring Vertex_shader_path, wstring Pixel_shader_path) :
//	Vertex_shader_path(Vertex_shader_path), Pixel_shader_path(Pixel_shader_path)
//{
//	//// rasterizer_desc
//	//rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
//	//rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
//}
//
//SHADER::SkinShader::~SkinShader() {}
//
//// Material을(를) 통해 상속됨
//
//void SHADER::SkinShader::load_resource(Engine* engine)
//{
//	__super::load_resource(engine);
//
//	vector<D3D11_INPUT_ELEMENT_DESC> Input_layout;
//	BTYPE::VB_MeshVertex::T_get_input_layout(Input_layout);
//	BTYPE::VB_BoneIDWeights::T_get_input_layout(Input_layout);
//	Vertex_shader.initialize(engine->Device.Get(), Vertex_shader_path, Input_layout);
//	Pixel_shader.initialize(engine->Device.Get(), Pixel_shader_path);
//}
//
//void SHADER::SkinShader::release_resource(Engine* engine)
//{
//	__super::release_resource(engine);
//
//	Vertex_shader.free_shader();
//	Pixel_shader.free_shader();
//}
//
//void SHADER::SkinShader::set_shader(ID3D11DeviceContext* Device_context)
//{
//	__super::set_shader(Device_context);
//
//	Device_context->IASetPrimitiveTopology(Topology);
//
//	Device_context->IASetInputLayout(Vertex_shader.GetInputLayout());
//	Device_context->VSSetShader(Vertex_shader.GetShader(), 0, 0);
//	Device_context->PSSetShader(Pixel_shader.GetShader(), 0, 0);
//}
//
//void SHADER::SkinShader::free_material(ID3D11DeviceContext* Device_context)
//{
//	__super::free_material(Device_context);
//
//	Device_context->IASetInputLayout(nullptr);
//	Device_context->VSSetShader(nullptr, 0, 0);
//	Device_context->PSSetShader(nullptr, 0, 0);
//}
//
//void SHADER::SkinShader::draw_detail_view()
//{
//	string Text;
//
//	if (ImGui::CollapsingHeader("Skin material"))
//	{
//		ImGuiWindowFlags Window_flags = 0;
//		Window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
//		ImGui::BeginChild("Skin material", ImVec2(0, 120), true, Window_flags);
//
//		Text = "Resource name : ";
//		if (this->Registed_name.get_string() == "")
//			ImGui::Text(Text.c_str());
//		else
//		{
//			Text += this->Registed_name.get_string();
//			ImGui::Text(Text.c_str());
//		}
//		ImGui::EndChild();
//	}
//}
//
