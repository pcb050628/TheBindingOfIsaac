#ifndef _STD2D
#define _STD2D

#include "values.fx"

struct VS_IN
{
    float3 vPos : POSITION; // Sementic
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);

    if(output.vPosition.z < 0)
    {
        output.vPosition.z = 0.1f;
    }
    
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // Texture Sampling, Mapping
    // float4 color = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    return _in.vColor;
}

#endif