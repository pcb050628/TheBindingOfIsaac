#ifndef _STD2D
#define _STD2D

#include "values.fx"
#include "struct.fx"
#include "func.fx"

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
    
    float3 vWorldPos : POSITION;
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
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // Texture Sampling, Mapping
    
    float4 vColor = _in.vColor;
    
    if(UseAnim2D)
    {
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vSliceSize / 2.f) - (g_vBackground / 2.f);
        vBackgroundLeftTop -= g_vOffset;
        float2 vUV = vBackgroundLeftTop + (g_vBackground * _in.vUV);
        
        if (vUV.x < g_vLeftTop.x || (g_vLeftTop.x + g_vSliceSize.x) < vUV.x
            || vUV.y < g_vLeftTop.y || (g_vLeftTop.y + g_vSliceSize.y) < vUV.y)
        {
            discard;
        }
        else
        {
            vColor = g_anim2d_tex.Sample(g_sam_1, vUV);
        }
    }
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
        else if (g_btex_1)
        {
            vColor = g_tex_1.Sample(g_sam_0, _in.vUV);
        }
        else if (g_btex_2)
        {
            vColor = g_tex_2.Sample(g_sam_0, _in.vUV);
        }
        else if (g_btex_3)
        {
            vColor = g_tex_3.Sample(g_sam_0, _in.vUV);        
        }
        else if (g_btex_4)
        {
            vColor = g_tex_4.Sample(g_sam_0, _in.vUV);
        }
        else if (g_btex_5)
        {
            vColor = g_tex_5.Sample(g_sam_0, _in.vUV);
        }
    //else if (g_btexcube_0)
    //{
    //    vColor = g_texcube_0.Sample(g_sam_0, _in.vUV);
    //    
    //}
    //else if (g_btexcube_1)
    //{
    //    vColor = g_texcube_1.Sample(g_sam_0, _in.vUV);
    //    
    //}
    //else if (g_btexarr_0)
    //{
    //    vColor = g_texarr_0.Sample(g_sam_0, _in.vUV);
    //    
    //}
    //else if (g_btexarr_1)
    //{
    //    vColor = g_texarr_1.Sample(g_sam_0, _in.vUV);
    //    
    //}
    }
    
    if (vColor.a <= 0.01f)
    {
        discard;
    }
    
    tLightColor LightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_Light2DCount; i++)
    {
        CalLight2D(_in.vWorldPos, i, LightColor);
    }
    
    vColor.rgb *= (LightColor.vColor.rgb + LightColor.vAmbient.rgb);
    
    return vColor;
}

#endif