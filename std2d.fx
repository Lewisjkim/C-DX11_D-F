#ifndef _STD2D
#define _STD2D
#include "value.fx"
#include "func.fx"
#define bAnimUse        g_int_0
#define LeftTop         g_vec2_0
#define Slice           g_vec2_1
#define Offset          g_vec2_2
#define BackSize        g_vec2_3
#define Gauge        g_float_0
#define GaugeMax        g_float_1
struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION;//*
};

// ============================
// Std2DShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// g_tex_0              : Output Texture
// ============================
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), g_matWorld).xyz;//*
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
        
    if (g_btex_0)//�̰� 0�̶��ؼ� �ȵ��ư�����
    {
        if (bAnimUse)
        {
            float2 vUV = LeftTop + (BackSize * _in.vUV);
            vUV -= ((BackSize - Slice) / 2.f);
            vUV -= Offset; //***
            
            if (LeftTop.x < vUV.x && vUV.x < LeftTop.x + Slice.x
                && LeftTop.y < vUV.y && vUV.y < LeftTop.y + Slice.y)
            {
                vOutColor = g_tex_0.Sample(g_sam_0, vUV);
            }
            else
            {
                vOutColor = float4(1.f, 1.f, 0.f, 0.f); //���� �÷� �����
                //discard;
            }
        }
        else
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
        
    float3 vNormal = (float3) 0.f;
    if (g_btex_1)
    {
        // Normal �� ����
        vNormal = g_tex_1.Sample(g_sam_0, _in.vUV);
        
        // 0 ~ 1 ������ -1 ~ 1 �� ����
        vNormal = (vNormal * 2.f) - 1.f;
        
        // NormalTexture ��ǥ��� y��� z ���� �ݴ�� �Ǿ��ִ�.
        float f = vNormal.y;
        vNormal.y = vNormal.z;
        vNormal.z = f;
        
        // Texture ���� ������ Normal ������ ����� ��ȯ��Ų��.
        vNormal = normalize(mul(float4(vNormal, 0.f), g_matWorld)).xyz;
    }
    
    
    
    if (0.f == vOutColor.a)//alpha�� 0�̸�
    {
        //return float4(1.f, 0.f, 1.f, 0.f); // �ִϸ��̼� ���� ����Ÿ
        discard;
    }
     
    
    // Lighting ó��
    tLightColor LightColor = (tLightColor) 0.f;
    
    // vNormal �� Zero Vector ���
    if (dot(vNormal, vNormal) == 0.f)
    {
        CalcLight2D(_in.vWorldPos, LightColor);
    }
    else
    {
        CalcLight2D(_in.vWorldPos, vNormal, LightColor);
    }
        
    vOutColor.rgb *= (LightColor.vDiffuse.rgb + LightColor.vAmbient.rgb);
    
    return vOutColor;
    //if (g_btex_0)
    //{
    //    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    //}
    //else
    //{
    //    vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    //}
        
    //if (0.f == vOutColor.a)
    //    discard;
    
    //if(g_int_2 == 1)
    //    vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    
    //return vOutColor;
}

struct G_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;//texture��ǥ(uv)�� RGBA�� �ؼ�
};

struct G_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION; 
};

VS_OUT VS_Gauge(G_IN _in)
{
    G_OUT output = (G_OUT) 0.f;
    float GaugeRatio = Gauge / GaugeMax;
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    
    output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), g_matWorld).xyz; //*
    
    return output;
}

float4 PS_Gauge(G_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
     //�÷� ���ø�
  
    float GaugeRatio = Gauge / GaugeMax;
    _in.vUV.y += 1-GaugeRatio;
    //_in.vWorldPos.y = _in.vWorldPos.y * GaugeRatio;
   //_in.vUV.y = _in.vUV.y*GaugeRatio ;//-�� �ϸ� �Ʒ��� �и��� �������ε�
    if (g_btex_0)
    {
        if(0<_in.vUV.y&&_in.vUV.y<1)
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
        
    if (0.f == vOutColor.a)
        discard;
    
    return vOutColor;
}

//=================
VS_OUT VS_Fade2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), g_matWorld).xyz; //*
    return output;
}

float4 PS_Fade2D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
      
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    else
    {
        vOutColor = float4(0.f, 0.f, 0.f, 0.f);
    }
   
        
    vOutColor.w += g_float_0;
        if (0.f >= vOutColor.a)
            discard;
    
    return vOutColor;
}


// ======================================
// Std2DLightShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter

//#define bAnimUse        g_int_0
//#define LeftTop         g_vec2_0
//#define Slice           g_vec2_1
//#define Offset          g_vec2_2
//#define BackSize        g_vec2_3

// g_tex_0              : Output Texture
// g_tex_1              : Nomal Texture
// ======================================

struct VS_Light_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_Light_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION;
};

VS_Light_OUT VS_Std2DLight(VS_Light_IN _in)
{
    VS_Light_OUT output = (VS_Light_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), g_matWorld).xyz;
    
        
    return output;
}

float4 PS_Std2DLight(VS_Light_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       
    if (g_vec4_0.x == 3.14f)
    {
        return float4(1.f, 0.f, 0.f, 1.f);
    }
    
    if (g_btex_0)//�̰� 0�̶��ؼ� �ȵ��ư�����
    {
        if (bAnimUse)
        {
            float2 vUV = LeftTop + (BackSize * _in.vUV);
            vUV -= ((BackSize - Slice) / 2.f);
            vUV -= Offset; //***
            
            if (LeftTop.x < vUV.x && vUV.x < LeftTop.x + Slice.x
                && LeftTop.y < vUV.y && vUV.y < LeftTop.y + Slice.y)
            {
                vOutColor = g_tex_0.Sample(g_sam_0, vUV);//���⿡�� ���ε��� �Ǿ��־���
            }
            else
            {
                vOutColor = float4(1.f, 1.f, 0.f, 0.f);                         //���� �÷� �����
                //discard;
            }
        }
        else
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
        
    float3 vNormal = (float3) 0.f;
    if (g_btex_1)
    {
        // Normal �� ����
        vNormal = g_tex_1.Sample(g_sam_0, _in.vUV);
        
        // 0 ~ 1 ������ -1 ~ 1 �� ����
        vNormal = (vNormal * 2.f) - 1.f;
        
        // NormalTexture ��ǥ��� y��� z ���� �ݴ�� �Ǿ��ִ�.
        float f = vNormal.y;
        vNormal.y = vNormal.z;
        vNormal.z = f;
        
        // Texture ���� ������ Normal ������ ����� ��ȯ��Ų��.
        vNormal = normalize(mul(float4(vNormal, 0.f), g_matWorld)).xyz;
    }
    
    
    
    if (0.f == vOutColor.a)//alpha�� 0�̸�
    {
        //return float4(1.f, 0.f, 1.f, 0.f); // �ִϸ��̼� ���� ����Ÿ
        discard;
    }
     
    
    // Lighting ó��
    tLightColor LightColor = (tLightColor) 0.f;
    
    // vNormal �� Zero Vector ���
    if (dot(vNormal, vNormal) == 0.f)
    {
        CalcLight2D(_in.vWorldPos, LightColor);
    }
    else
    {
        CalcLight2D(_in.vWorldPos, vNormal, LightColor);
    }
        
    vOutColor.rgb *= (LightColor.vDiffuse.rgb + LightColor.vAmbient.rgb);
    
    return vOutColor;
}

#endif