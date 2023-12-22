#ifndef _STD2D
#define _STD2D

// 4096
cbuffer TRANSFORM : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matWorldInv;

    row_major matrix g_matView;
    row_major matrix g_matViewinv;

    row_major matrix g_matProj;
    row_major matrix g_matProjInv;
    
    row_major matrix g_matWV;
    row_major matrix g_matWVP;
}

Texture2D g_tex_0 : register(t0);

SamplerState g_sam_0 : register(s0);

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