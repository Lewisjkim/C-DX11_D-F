#include "pch.h"
#include "global.h"

////extern(전역)만 모아두는cpp 
tTransform g_transform = {};
tGlobal GlobalData = {};

extern const char* RES_TYPE_STR[(UINT)RES_TYPE::END] =
{
	"MESHDATA",
	"MATERIAL",
	"PREFAB",
	"MESH",
	"TEXTURE",
	"SOUND",
	"GRAPHICS_SHADER",
	"COMPUTE_SHADER"
};

extern const wchar_t* RES_TYPE_WSTR[(UINT)RES_TYPE::END] =
{
	L"MESHDATA",
	L"MATERIAL",
	L"PREFAB",
	L"MESH",
	L"TEXTURE",
	L"SOUND",
	L"GRAPHICS_SHADER",
	L"COMPUTE_SHADER"
};

extern const char* COMPONENT_TYPE_STR[(UINT)COMPONENT_TYPE::END] =
{
	"TRANSFORM",
	"COLLIDER2D",
	"COLLIDER3D",
	"ANIMATOR2D",
	"ANIMATOR3D",
	"LIGHT2D",
	"LIGHT3D",
	"CAMERA",
	"MESHRENDER",
	"PARTICLESYSTEM",
	"TILEMAP",
	"LANDSCAPE",
	"DECAL",
};

extern const wchar_t* COMPONENT_TYPE_WSTR[(UINT)COMPONENT_TYPE::END] =
{
	L"TRANSFORM",
	L"COLLIDER2D",
	L"COLLIDER3D",
	L"ANIMATOR2D",
	L"ANIMATOR3D",
	L"LIGHT2D",
	L"LIGHT3D",
	L"CAMERA",
	L"MESHRENDER",
	L"PARTICLESYSTEM",
	L"TILEMAP",
	L"LANDSCAPE",
	L"DECAL",
};

extern const char* RS_TYPE_STR[(UINT)RS_TYPE::END] =
{
	"CULL_BACK",
	"CULL_FRONT",
	"CULL_NONE",
	"WIRE_FRAME"
};
extern const wchar_t* RS_TYPE_WSTR[(UINT)RS_TYPE::END] =
{
	L"CULL_BACK",
	L"CULL_FRONT",
	L"CULL_NONE",
	L"WIRE_FRAME"
};

extern const char* DS_TYPE_STR[(UINT)DS_TYPE::END] =
{
	"LESS",
	"LESS_EQUAL",
	"GREATER",
	"GREATER_EQUAL",
	"NO_WRITE",			
	"NO_TEST_NO_WRITE"
};
extern const wchar_t* DS_TYPE_WSTR[(UINT)DS_TYPE::END] =
{
	L"LESS",
	L"LESS_EQUAL",
	L"GREATER",
	L"GREATER_EQUAL",
	L"NO_WRITE",
	L"NO_TEST_NO_WRITE"
};

extern const char* BS_TYPE_STR[(UINT)BS_TYPE::END] =
{
	"DEFAULT",	
	"MASK",		
	"ALPHA_BLEND",	
	"ONE_ONE"
};
extern const wchar_t* BS_TYPE_WSTR[(UINT)BS_TYPE::END] =
{
	L"DEFAULT",
	L"MASK",
	L"ALPHA_BLEND",
	L"ONE_ONE"
};

extern const char* DOMAIN_TYPE_STR[(UINT)SHADER_DOMAIN::END] =
{
	"DOMAIN_OPAQUE",		
	"DOMAIN_MASK",		
	"DOMAIN_TRANSPARENT",	
	"DOMAIN_POSTPROCESS", 
	"DOMAIN_UI",
	"DOMAIN_UNDEFINED"
};
extern const wchar_t* DOMAIN_TYPE_WSTR[(UINT)SHADER_DOMAIN::END] =
{
	L"DOMAIN_OPAQUE",
	L"DOMAIN_MASK",
	L"DOMAIN_TRANSPARENT",
	L"DOMAIN_POSTPROCESS",
	L"DOMAIN_UI",
	L"DOMAIN_UNDEFINED"
};