struct PS_INPUT
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 texture_coordinate : TEXCOORD;
    float3 world_position : COLOR0;
    float4 vertex_color_0 : COLOR1;
    float4 vertex_color_1 : COLOR2;
    float4 vertex_color_2 : COLOR3;
};