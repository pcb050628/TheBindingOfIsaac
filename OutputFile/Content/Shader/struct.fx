#ifndef _STRUCT
#define _STRUCT

struct tLightInfo
{
    float4 vColor;
    float4 vSpecular;
    float4 vAmbient;

    float3 vWorldPos;
    float3 vWorldDir;
    float fRadius;
    float fAngle;

    int LightType;

    float3 vPadding;
};

#endif