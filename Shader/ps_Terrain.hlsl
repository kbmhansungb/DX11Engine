#pragma pack_matrix(row_major)
#include "_PS_INPUT.fx"
#include "_DF_INPUT_RENDERING.fx"

// 0 - 0 디퓨즈를 의미
Texture2D terrain_color_texture[9] : register(t0);
SamplerState terrain_color_sampler : register(s0);

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
            += terrain_color_texture[i].Sample(terrain_color_sampler, input.texture_coordinate)
            * texture_weight[i];
        }
    }
    
    DF_INPUT output;
    output.base_color = result_color;
    output.normal = float4(input.normal, 1.0f);
    output.world_position = float4(input.world_position.xyz, 1.0f);
    
    return output;
}