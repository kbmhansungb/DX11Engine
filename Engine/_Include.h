#pragma once

//////
// 엔진 개요
// https://docs.google.com/presentation/d/1myrzT0gSBAm4Yu58ifUpS0y4pMSNwRtS0oKgWJ8000M/edit#slide=id.ge8fa8da02e_0_0
//


//////
// Window
#include "Window/_Exclude.h"
// byte를 대체 어디서 정의하는거지????,

//////
// standard lib
#include <string>
#include <memory>
#include <vector>
#include <list>
#include <ppl.h>
#include <algorithm>
#include <random>
#include <math.h>
#include <typeindex>
//using namespace concurrency;
//using namespace std;


//////
// Direct X
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")
#include <DirectXMath.h>
#include <wrl/client.h>
#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")
//using namespace DirectX;
//using namespace Microsoft::WRL;

//////
// Assimp

#include <ScreenGrab.h>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
//#include <wincodec.h>

//////
// ImGui
#include "ImGui_Sort/_Exclude.h"

//////
// 사용하는 정의는 여기로 와야 한다.
#include "Template/EditorWindow/EditorDefineContainer.h"
#include "Component/Graphics/GraphicsDefine.h"

//////
// setting
#define TERRAIN_TEXTURE_SIZE		9

#define DEFINE_MAX_BONE_SIZE		256
#define DEFINE_MAX_BONE_LINK_SIZE	8