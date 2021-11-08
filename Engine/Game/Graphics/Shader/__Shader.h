#pragma once

#include "../../_Include.h"
#include <string.h>
//using namespace std;
#pragma comment(lib,"D3DCompiler.lib") // 이걸 지우고 왜 안되는지 찾고 있었네..
#include <d3dcompiler.h>

namespace SHADER
{
	std::wstring GetShaderPath(std::wstring shader_name);
}

namespace SHADER
{
	class VertexShader
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
		Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	public:
		virtual ~VertexShader();
		ID3D11VertexShader* GetShader();
		ID3D10Blob* GetBuffer();
		ID3D11InputLayout* GetInputLayout();
	public:
		void initialize(ID3D11Device* device, std::wstring shader_name, std::vector<D3D11_INPUT_ELEMENT_DESC>& Input_layout);
		void free_shader();
	};
}
namespace SHADER
{
	class PixelShader
	{
		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
		Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
	public:
		virtual ~PixelShader();
		ID3D11PixelShader* GetShader();
		ID3D10Blob* GetBuffer();
	public:
		void initialize(ID3D11Device* device, std::wstring shader_name);
		void free_shader();
	};
}
namespace SHADER
{
	class GeometryShader
	{
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> shader;
		Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
	public:
		virtual ~GeometryShader();
		ID3D11GeometryShader* GetShader();
		ID3D10Blob* GetBuffer();
	public:
		void initialize(ID3D11Device* device, std::wstring shader_name);
		void free_shader();
	};
}