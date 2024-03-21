#ifndef _SETCOLOR
#define _SETCOLOR

#include "value.fx"

RWTexture2D<float4> g_Output : register(u0); //texture uav���� ������ ����


// ComputeShader �Է� Sematic
// SV_GroupThreadID      : �׷� �������� �ε���(3����)
// SV_GroupID            : �׷� �ε���
// SV_GroupIndex         : �׷� �������� �ε���(1����)
// SV_DispatchThreadID   : ��ü �׷� ����, �������� �ε���(3����)

//�ѹ��� ó���� thread �׷� ���� 1024��(�ִ�ġ)=(hlsl5����)
[numthreads(32, 32, 1)]
void CS_SetColor(int3 _ID : SV_DispatchThreadID)//id = thread�� ���� ��ȣ = ���� ���� ��
{
    //compute shader���ڱⰡ ����� �ȼ��� ������ߵȴ�
    if (380 <= _ID.y && _ID.y <= 388 || 636 <= _ID.x && _ID.x <= 644)//���ڰ� ���
    {
        g_Output[_ID.xy] = float4(1.f, 1.f, 0.f, 1.f);//�����
    }
}



#endif