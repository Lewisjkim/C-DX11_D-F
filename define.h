#pragma once
#include <chrono>

//매크로와 전역 변수
#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }

#define KEY_TAP(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::TAP		
#define KEY_RELEASE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::RELEASE
#define KEY_PRESSED(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::PRESSED
#define KEY_NONE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::NONE

#define DT CTimeMgr::GetInst()->GetDeltaTime()
#define FPS CTimeMgr::GetInst()->GetFPS()
#define WAIT CTimeMgr::GetInst()->Wait(float _time)
#define MAX_LAYER 32

#define SINGLE(type) private: type(); ~type(); friend class CSingleton<type>;

enum class COMPONENT_TYPE
{
	// update
	TRANSFORM,		// 위치, 크기, 회전
	COLLIDER2D,		// 2차원 충돌
	COLLIDER3D,		// 3차원 충돌
	ANIMATOR2D,		// Sprite Animation
	ANIMATOR3D,		// Bone Sknning Animation
	LIGHT2D,		// 2차원 광원
	LIGHT3D,		// 3차원 광원
	CAMERA,			// Camera

	// render
	MESHRENDER,		// 기본적인 렌더링
	PARTICLESYSTEM, // 입자 렌더링
	TILEMAP,		// 2차원 타일
	LANDSCAPE,		// 3차원 지형
	DECAL,			// 내부 렌더링

	END,
	// custom
	SCRIPT,
};


enum class RES_TYPE
{
	MESHDATA,		//모양
	MATERIAL,		//재질
	PREFAB,

	MESH,			// 형태
	TEXTURE,		// 이미지
	SOUND,

	GRAPHICS_SHADER,
	COMPUTE_SHADER,

	END,
};

extern const char* RES_TYPE_STR[(UINT)RES_TYPE::END];
extern const wchar_t* RES_TYPE_WSTR[(UINT)RES_TYPE::END];

extern const char* COMPONENT_TYPE_STR[(UINT)COMPONENT_TYPE::END]; //여기에 정의하고
extern const wchar_t* COMPONENT_TYPE_WSTR[(UINT)COMPONENT_TYPE::END];

enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2

	END,
};

enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

enum TEX_PARAM // 텍스쳐 변수
{
	TEX_0,  //t0
	TEX_1,  //t1
	TEX_2,  //t2
	TEX_3,  //t3
	TEX_4,  //t4
	TEX_5,  //t5
	TEX_6,  //t6
	TEX_7,  //t7

	TEX_CUBE_0, //t8
	TEX_CUBE_1, //t9

	TEX_ARR_0,//t10
	TEX_ARR_1,//t11

	TEX_END,
};

enum PIPELINE_STAGE
{
	PS_VERTEX = 0x01,
	PS_HULL = 0x02,
	PS_DOMAIN = 0x04,
	PS_GEOMETRY = 0x08,
	PS_PIXEL = 0x10,

	PS_ALL = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY | PS_PIXEL,
};

enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};
extern const char* RS_TYPE_STR[(UINT)RS_TYPE::END];
extern const wchar_t* RS_TYPE_WSTR[(UINT)RS_TYPE::END];

enum class DS_TYPE // depth stencil stage
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_WRITE,			// LESS, DepthWrite X
	NO_TEST_NO_WRITE,	// Test X, DepthWrite X
	END,
};
extern const char* DS_TYPE_STR[(UINT)DS_TYPE::END];
extern const wchar_t* DS_TYPE_WSTR[(UINT)DS_TYPE::END];


enum class BS_TYPE // Blending stage
{
	DEFAULT,		// No Blending
	MASK,			// Alpha Coverage
	ALPHA_BLEND,	// Alpha 계수 
	ONE_ONE,		// 1:1 혼합
	END,
};
extern const char* BS_TYPE_STR[(UINT)BS_TYPE::END];
extern const wchar_t* BS_TYPE_WSTR[(UINT)BS_TYPE::END];

enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
};

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

enum class SHADER_DOMAIN //shader 영역 설정
{
	DOMAIN_OPAQUE,		// 불투명 오브젝트 3D
	DOMAIN_MASK,		// 불투명, 투명 - Pixel shader에서 discard로 중간에 깊이를 뺴는 것
	DOMAIN_TRANSPARENT,	// 반투명

	DOMAIN_POSTPROCESS, // 후 처리
	DOMAIN_UI,

	DOMAIN_UNDEFINED,	// 미정
	END, //추가함
};

extern const char* DOMAIN_TYPE_STR[(UINT)SHADER_DOMAIN::END];
extern const wchar_t* DOMAIN_TYPE_WSTR[(UINT)SHADER_DOMAIN::END];
enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,

	ADD_CHILD,						

	DELETE_RESOURCE,			// wParam : RES_TYPE, lParam : Resource Adress

	CHANGE_NAME,
	LEVEL_CHANGE,
};

enum class SHAPE_TYPE
{
	RECT,
	CIRCLE,
	CUBE,
	SPHERE,
	END,
};


enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,	// 방향성(광원에 방향값을 넣은것. 고로 위치는 중요하지 않다)
	POINT,			// 지점을 중심으로부터 빛이 나가는 빛
	SPOT,			// 어느 지점만 빛이 나오는 것
};

enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};

enum class SB_TYPE
{
	READ_ONLY,
	READ_WRITE,
};

enum class PARTICLE_MODULE
{
	PARTICLE_SPAWN, //파티클 생성 모듈
	COLOR_CHANGE, // 파티클 컬러 변경 모듈
	SCALE_CHANGE, // 파티클 크기 변경 모듈
	ADD_VELOCITY,

	DRAG,						//일정 시간이 지나면 속도가 점점 줄일 수 있는 모듈
	NOISE_FORCE,
	RENDER,
	DUMMY_3,

	END,
};

enum class PlayerDir
{
	Right,
	Left
};