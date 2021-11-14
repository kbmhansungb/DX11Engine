#pragma pack_matrix(row_major)
#include "_PS_INPUT.fx"
#include "_DF_INPUT_RENDERING.fx"

Texture2D base_color_texture : register(t0);
SamplerState base_color_sampler : register(s0);

DF_INPUT ps(PS_INPUT input)
{
    DF_INPUT output;
    float depth = input.position.w / input.position.z;
    
    output.base_color = base_color_texture.Sample(base_color_sampler, input.texture_coordinate);
    output.normal = float4(input.normal, 1.0f);
    output.world_position = float4(input.world_position.xyz, 1.0f);
    output.depth = float4(depth, depth, depth, 1.0f);
    
    return output;
}