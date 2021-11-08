#pragma pack_matrix(row_major)
#include "_PS_INPUT.fx"
#include "_DF_INPUT_LIGHTING.fx"

#include "_CONSTANT_10_LIGHT.fx"
#include "_CONSTANT_11_LIGHT_POS.fx"

// 말이 
cbuffer CAM_VIEW_PROJECTION : register(b12)
{
    matrix cam_view_projection =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    matrix cam_view_projection_inverse =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
};
cbuffer CAM_WORLD : register(b13)
{
    matrix cam_world =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    matrix cam_world_inverse =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
};

// light에서 넘어옴.
Texture2D depth_texture : register(t0);
SamplerState depth_sampler : register(s0);

Texture2D light_texture : register(t1);
SamplerState light_sampler : register(s1);

// render to texture에서 넘어옴.
Texture2D world_position_texture : register(t2);
SamplerState world_position_sampler : register(s2);

Texture2D normal_texture : register(t3);
SamplerState normal_sampler : register(s3);

// 픽셀별로 라이팅을 처리하는데...
DF_INPUT ps(PS_INPUT input)
{
    float bias = 0.001f;
    DF_INPUT output;
    
    float3 wp = world_position_texture.Sample(world_position_sampler, input.texture_coordinate).xyz;

    float4 light_sv_pos = mul(float4(wp, 1.0f), cam_view_projection);
    float2 proj_tex;
    proj_tex.x = light_sv_pos.x / light_sv_pos.w / 2.0f + 0.5f;
    proj_tex.y = -light_sv_pos.y / light_sv_pos.w / 2.0f + 0.5f;
    float light_depth_value = distance(light_pos, wp);
    
    if ((saturate(proj_tex.x) == proj_tex.x) &&
        (saturate(proj_tex.y) == proj_tex.y))
    {
        float depth_value = depth_texture.Sample(depth_sampler, proj_tex).x;
        light_depth_value -= bias;// * light_depth_value;
        
        if (light_depth_value < depth_value)
        {
            //float weight = dot(-light_vector.xyz, normal_texture.Sample(normal_sampler, input.texture_coordinate).xyz);
            
            //if(weight > 0.f)
            //{
            //    output.light_weight = light_texture.Sample(light_sampler, proj_tex) * weight;
            //    output.light_weight.a = 1.f;
            //}
            //else
            //{
            //    output.light_weight = float4(0.f, 0.f, 0.f, 1.f);
            //}
            
            
            output.light_weight = light_texture.Sample(light_sampler, proj_tex);
            output.light_weight.a = 1.f;
        }
        else
        {
            output.light_weight = float4(0.f, 0.f, 0.f, 1.f);
        }
    }
    else
    {
        output.light_weight = float4(0.f, 0.f, 0.f, 1.f);
    }
    return output;
}