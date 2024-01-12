#ifndef _FUNC
#define _FUNC


#include "struct.fx"
#include "values.fx"


void CalLight2D(float3 _WorldPos, int _LightIdx, inout tLightColor _output)
{
    // 빛을 적용시킬 광원의 정보
    tLightInfo info = g_Light2D[_LightIdx];
    
    // Directional Light
    if (0 == info.LightType)
    {
        _output.vAmbient += info.Color.vAmbient;
    }
    
    // Point Light
    else if (1 == info.LightType)
    {
        float fAttenu = 1.f;
        
        float fDist = distance(info.vWorldPos.xy, _WorldPos.xy);
        if (fDist < info.fRadius)
        {
            if (g_int_0)
            {
                float fTheta = (fDist / info.fRadius) * (PI / 2.f);
                fAttenu = saturate(cos(fTheta));
            }
            else
            {
                fAttenu = saturate(1.f - fDist / g_Light2D[0].fRadius);
            }
            
            _output.vColor += info.Color.vColor * fAttenu;
        }
    }
    
    // Spot Light
    else
    {
        float fDist = distance(_WorldPos.xy, info.vWorldPos.xy);
        
        float2 vec1 = normalize(_WorldPos.xy - info.vWorldPos.xy);
        float2 vec2 = normalize(info.vWorldDir.xy);
        
        float fTheta = acos(dot(vec1, vec2));
        
        float RadToDeg = fTheta * (180 / PI);
        
        if (  RadToDeg > -(info.fAngle / 2)
           && RadToDeg < (info.fAngle / 2)
           && fDist < info.fRadius)
        {
            _output.vColor += info.Color.vColor;
        }
    }
}

#endif