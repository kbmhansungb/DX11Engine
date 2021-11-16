#pragma once

#include "../../_Include.h"
#include <string.h>

// 쉐이더를 컴파일 하기위해서 필요함.
// 21.11.16
#pragma comment(lib,"D3DCompiler.lib")
#include <d3dcompiler.h>

/*
* 버텍스 쉐이더와 픽셀 쉐이더로 나누어짐.
* 메서드로 GetShaderPath를 가지고 있음.
* 21.11.16
*/
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