#ifndef _SETCOLOR
#define _SETCOLOR

#include "value.fx"

RWTexture2D<float4> g_Output : register(u0); //texture uav에서 가져온 정보


// ComputeShader 입력 Sematic
// SV_GroupThreadID      : 그룹 내에서의 인덱스(3차원)
// SV_GroupID            : 그룹 인덱스
// SV_GroupIndex         : 그룹 내에서의 인덱스(1차원)
// SV_DispatchThreadID   : 전체 그룹 기준, 스레드의 인덱스(3차원)

//한번에 처리할 thread 그룹 개수 1024개(최대치)=(hlsl5기준)
[numthreads(32, 32, 1)]
void CS_SetColor(int3 _ID : SV_DispatchThreadID)//id = thread의 고유 번호 = 제일 많이 씀
{
    //compute shader가자기가 담당할 픽셀을 정해줘야된다
    if (380 <= _ID.y && _ID.y <= 388 || 636 <= _ID.x && _ID.x <= 644)//십자가 모양
    {
        g_Output[_ID.xy] = float4(1.f, 1.f, 0.f, 1.f);//노랑색
    }
}



#endif