#pragma pack_matrix(row_major)
#include "_PS_INPUT.fx"
#include "_DF_INPUT_LIGHT_PROCESS.fx"

cbuffer LightProcess : register(b13)
{
    uint sample_count;
}

Texture2D base_color_texture : register(t0);
SamplerState base_color_sampler : register(s0);
Texture2D light_weight_texture : register(t1);
SamplerState light_weight_sampler : register(s1);
Texture2D world_position_texture : register(t2);
SamplerState world_position_sampler : register(s2);

// 이렇게 하는게 아닌거 같다..
//#define SAMPLE_COUNT_ROOT 2
DF_INPUT ps(PS_INPUT input)
{
    DF_INPUT output;
    
    #define Mode 0
    #if Mode == 0
    
    uint width, height, level;
    light_weight_texture.GetDimensions(0, width, height, level);
    float tex_x = 1.f / width, tex_y = 1.f / height;
    float3 wp = world_position_texture.Sample(world_position_sampler, input.texture_coordinate).rgb;
    
    float weight = 0;
    float3 color = 0;
    
    float sc = sample_count;
    
    for (int x = -sc; x <= sc; ++x)
        for (int y = -sc; y <= sc; ++y)
        {
            float2 tex = float2(input.texture_coordinate.x + x * tex_x, input.texture_coordinate.y + y * tex_y);
            color += light_weight_texture.Sample(light_weight_sampler, tex).rgb;
        }
    color /= (sc * 2 + 1.0f) * (sc * 2 + 1.0f);
    
    float3 base_color = base_color_texture.Sample(base_color_sampler, input.texture_coordinate).rgb;
    output.result_color = float4(color * base_color, 1.f);
    
    #endif
    #if Mode == 1
    
    float3 base_color = base_color_texture.Sample(base_color_sampler, input.texture_coordinate).rgb;
    float3 light_weight = light_weight_texture.Sample(light_weight_sampler, input.texture_coordinate).rgb;
    
    output.result_color = float4(base_color * light_weight, 1.0f);
    
    #endif
    
    return output;
}