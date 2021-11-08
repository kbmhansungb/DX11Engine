#pragma pack_matrix(row_major)
#include "_PS_INPUT.fx"
#include "_DF_INPUT_LIGHT_PROCESS.fx"

cbuffer TEXEL_SIZE : register(b13)
{
    float2 texel_size;
}

// Render to texture에서 설정됨.
Texture2D base_color_texture : register(t0);
SamplerState base_color_sampler : register(s0);
Texture2D light_weight_texture : register(t1);
SamplerState light_weight_sampler : register(s1);

// 이렇게 하는게 아닌거 같다..
//#define SAMPLE_COUNT_ROOT 2
DF_INPUT ps(PS_INPUT input)
{
    DF_INPUT output;
    float4 base_color = base_color_texture.Sample(base_color_sampler, input.texture_coordinate);
    float4 light_weight = light_weight_texture.Sample(light_weight_sampler, input.texture_coordinate);
    
    output.result_color = float4(base_color.xyz * light_weight.xyz, 1.0f);

    
    //output.result_color = float4(0.f, 0.f, 0.f, 1.0f);
    //for (int i = -SAMPLE_COUNT_ROOT; i < SAMPLE_COUNT_ROOT; ++i)
    //{
    //    for (int j = -SAMPLE_COUNT_ROOT; j < SAMPLE_COUNT_ROOT; ++j)
    //    {
    //        float4 light_weight 
    //            = light_weight_texture.Sample(light_weight_sampler, 
    //            float2(input.texture_coordinate.x - i * texel_size.x, input.texture_coordinate.y - j * texel_size.y));
    //        output.result_color.xyz += base_color.xyz * light_weight.xyz / 9.f / 9.f;
    //    }
    //}
    
    return output;
}