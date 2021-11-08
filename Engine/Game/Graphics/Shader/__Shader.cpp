#include "__Shader.h"
using namespace std;

wstring SHADER::GetShaderPath(wstring shader_name)
{
	wchar_t path[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, path, MAX_PATH);

	wstring shaderfolder = path;// L"";
	size_t size = shaderfolder.size();
	size_t last = shaderfolder.find_last_of(L'\\');
	shaderfolder = shaderfolder.substr(0, last);
	shaderfolder += L"\\Shader\\";
	//#ifdef _DEBUG
	//#ifdef _WIN64
	//		//shaderfolder = L"../x64/Debug/Shader/";
	//		shaderfolder = L"Shader/";
	//#else
	//		//shaderfolder = L"../Debug/Shader/";
	//		shaderfolder = L"Shader/";
	//#endif
	//#else
	//#ifdef _WIN64
	//		//shaderfolder = L"../x64/Release/Shader/";
	//		shaderfolder = L"Shader/";
	//#else
	//		//shaderfolder = L"../Release/Shader/";
	//		shaderfolder = L"Shader/";
	//#endif
	//#endif
	return shaderfolder + shader_name + L".cso";
}

SHADER::VertexShader::~VertexShader()
{
}

ID3D11VertexShader* SHADER::VertexShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob* SHADER::VertexShader::GetBuffer()
{
	return this->shader_buffer.Get();
}

ID3D11InputLayout* SHADER::VertexShader::GetInputLayout()
{
	return this->inputLayout.Get();
}

void SHADER::VertexShader::initialize(ID3D11Device* device, std::wstring shader_name, std::vector<D3D11_INPUT_ELEMENT_DESC>& Input_layout)
{
	shader.Reset();
	shader_buffer.Reset();
	inputLayout.Reset();

	wstring shader_path = GetShaderPath(shader_name);
	try
	{
		HRESULT hr;
		hr = D3DReadFileToBlob(
			shader_path.c_str(),
			this->shader_buffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to vertex shader buffer.\n" + shader_path);

		hr = device->CreateVertexShader(
			this->shader_buffer.Get()->GetBufferPointer(),
			this->shader_buffer.Get()->GetBufferSize(),
			0,
			this->shader.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create vertex shader.");

		hr = device->CreateInputLayout(
			Input_layout.data(),
			Input_layout.size(),
			this->shader_buffer->GetBufferPointer(),
			this->shader_buffer->GetBufferSize(),
			this->inputLayout.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create input layout.");
	}
	catch (COMException& exp)
	{
		ErrorLogger::Log(exp);
		return;
	}
}

void SHADER::VertexShader::free_shader()
{
	shader.Reset();
	shader_buffer.Reset();
	inputLayout.Reset();
}

SHADER::PixelShader::~PixelShader()
{
}

ID3D11PixelShader* SHADER::PixelShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob* SHADER::PixelShader::GetBuffer()
{
	return this->shader_buffer.Get();
}

void SHADER::PixelShader::initialize(ID3D11Device* device, wstring shader_name)
{
	shader.Reset();
	shader_buffer.Reset();
	wstring shader_path = GetShaderPath(shader_name);

	try
	{
		HRESULT hr;
		hr = D3DReadFileToBlob(
			shader_path.c_str(),
			this->shader_buffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to pixel shader buffer.\n" + shader_path);

		hr = device->CreatePixelShader(
			this->shader_buffer.Get()->GetBufferPointer(),
			this->shader_buffer.Get()->GetBufferSize(),
			0,
			this->shader.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create vertex shader.");
	}
	catch (COMException& exp)
	{
		ErrorLogger::Log(exp);
		return;
	}
}

void SHADER::PixelShader::free_shader()
{
	shader.Reset();
	shader_buffer.Reset();
}

SHADER::GeometryShader::~GeometryShader()
{
}

ID3D11GeometryShader* SHADER::GeometryShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob* SHADER::GeometryShader::GetBuffer()
{
	return this->shader_buffer.Get();
}

void SHADER::GeometryShader::initialize(ID3D11Device* device, std::wstring shader_name)
{
	shader.Reset();
	shader_buffer.Reset();
	wstring shader_path = GetShaderPath(shader_name);

	try
	{
		HRESULT hr;
		hr = D3DReadFileToBlob(
			shader_path.c_str(),
			this->shader_buffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to pixel shader buffer.\n" + shader_path);

		hr = device->CreateGeometryShader(
			this->shader_buffer.Get()->GetBufferPointer(),
			this->shader_buffer.Get()->GetBufferSize(),
			0,
			this->shader.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create vertex shader.");
	}
	catch (COMException& exp)
	{
		ErrorLogger::Log(exp);
		return;
	}
}

void SHADER::GeometryShader::free_shader()
{
	shader.Reset();
	shader_buffer.Reset();
}
