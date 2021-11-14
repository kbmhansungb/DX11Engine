#pragma pack_matrix(row_major)
#include "_PS_INPUT.fx"
#include "_DF_INPUT_LIGHT_STENCIL_SVTARGET.fx"

#include "_CONSTANT_10_LIGHT.fx"

Texture2D base_color_texture : register(t0);
SamplerState base_color_sampler : register(s0);

// render target의 값을 가져오는건 불가능 하다.
DF_INPUT ps(PS_INPUT input)
{
    DF_INPUT output;
    //output.Color0 = float4(light_color * light_strength, 1.0f);
    float far = distance(light_pos, (float3) input.world_position);
    float4 base_color = base_color_texture.Sample(base_color_sampler, input.texture_coordinate);
    
    if (base_color.a >= 1.f)
    { 
        output.depth.r = far;
        output.depth.g = far;
        output.depth.b = far;
        output.depth.a = 1.f;
    }
    else
    {
        output.depth.r = 0.f;
        output.depth.g = 0.f;
        output.depth.b = 0.f;
        output.depth.a = 0.f;
    }
    
    return output;
}