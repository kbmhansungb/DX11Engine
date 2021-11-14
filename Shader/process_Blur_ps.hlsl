#pragma pack_matrix(row_major)
#include "_PS_INPUT.fx"
#include "_DF_INPUT_LIGHT_PROCESS.fx"

cbuffer BLUR : register(b13)
{
    float min;
    float middle;
    float max;
    uint sample_size;
}

// Render to texture에서 설정됨.
Texture2D result_color_texture : register(t0);
SamplerState result_color_sampler : register(s0);

Texture2D depth_texture : register(t1);
SamplerState depth_sampler : register(s1);

// 이렇게 하는게 아닌거 같다..
//#define SAMPLE_COUNT_ROOT 2
DF_INPUT ps(PS_INPUT input)
{
    DF_INPUT output;
    output.result_color = float4(0.f, 0.f, 0.f, 1.f);
    
    float depth = depth_texture.Sample(depth_sampler, input.texture_coordinate);
    
    uint width, height, level;
    result_color_texture.GetDimensions(0, width, height, level);
    float tex_x = 1.f / width, tex_y = 1.f / height;
    
    // no depth blur
    float3 original_color = result_color_texture
                .Sample(result_color_sampler, input.texture_coordinate);
    float3 blur_color = 0;
    for (float x = -0.5f - sample_size; x <= 0.5f + sample_size; ++x)
        for (float y = -0.5f - sample_size; y <= 0.5f + sample_size; ++y)
        {
            blur_color += result_color_texture
                .Sample(result_color_sampler, float2(x * tex_x + input.texture_coordinate.x, y * tex_y + input.texture_coordinate.y));
        }
    blur_color /= (sample_size + 2) * (sample_size + 2);
    
    
    if (min > depth)
    {
        //output.result_color.rgb = 1.f;
        output.result_color.rgb = blur_color;
    }
    else if (middle > depth)
    {
        //output.result_color.rgb = (1.f - (depth - min) / middle);
        output.result_color.rgb = lerp(original_color, blur_color, 
        (1.f - (depth - min) / middle));
    }
    else if (depth > max)
    {
        //output.result_color.rgb = (1.f);
        output.result_color.rgb = blur_color;
    }
    else
    {
        //output.result_color.rgb = (depth - middle) / max;
        output.result_color.rgb = lerp(original_color, blur_color,
        (depth - middle) / max);
    }   
    
    return output;
}