cbuffer CB_LIGHT : register(b10)
{
    float3 light_color;
    float light_strength;
    float4 light_vec;
    float3 light_pos;
    float a;
    float b;
    float3 ambient_color;
    float ambient_strength;
};