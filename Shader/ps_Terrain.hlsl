#pragma pack_matrix(row_major)
#include "_PS_INPUT.fx"
#include "_DF_INPUT_RENDERING.fx"

cbuffer POS : register(b9)
{
    float3 m_pos;
    float b_size;
};

// 0 - 0 디퓨즈를 의미
Texture2D terrain_color_texture[9] : register(t0);
SamplerState terrain_color_sampler[9] : register(s0);

Texture2D brush_shape       : register(t10);
SamplerState brush_sampler  : register(s10);

DF_INPUT ps(PS_INPUT input)
{
    float4 result_color = 0;
    
    float texture_weight[9];
    texture_weight[0] = input.vertex_color_0.x;
    texture_weight[1] = input.vertex_color_0.y;
    texture_weight[2] = input.vertex_color_0.z;
    texture_weight[3] = input.vertex_color_0.w;
    texture_weight[4] = input.vertex_color_1.x;
    texture_weight[5] = input.vertex_color_1.y;
    texture_weight[6] = input.vertex_color_1.z;
    texture_weight[7] = input.vertex_color_1.w;
    texture_weight[8] = input.vertex_color_2.x;
    for (int i = 0; i < 9; ++i)
    {
        if (texture_weight[i] > 0.0f)
        {
            result_color
            += terrain_color_texture[i].Sample(terrain_color_sampler[i], input.texture_coordinate)
            * texture_weight[i];
        }
    }
    
    DF_INPUT output;
    output.base_color = result_color;
    output.normal = float4(input.normal, 1.0f);
    output.world_position = float4(input.world_position.xyz, 1.0f);
    
    // 브러시 그리기.
    if (b_size >= 0.01f)
    {
        float2 depos;
        depos.x = (output.world_position.x - m_pos.x + b_size / 2.f) / b_size;
        depos.y = (output.world_position.z - m_pos.z + b_size / 2.f) / b_size;
        if (depos.x == saturate(depos.x) && depos.y == saturate(depos.y))
        {
            float4 add = brush_shape.Sample(brush_sampler, depos);
            output.base_color.xyz += add.r;
        }
    }
    
    return output;
}