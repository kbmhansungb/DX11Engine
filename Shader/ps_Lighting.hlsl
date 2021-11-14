#pragma pack_matrix(row_major)
#include "_PS_INPUT.fx"
#include "_DF_INPUT_LIGHTING.fx"

#include "_CONSTANT_10_LIGHT.fx"

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
SamplerComparisonState cmpsam : register(s0);

// render to texture에서 넘어옴.
Texture2D world_position_texture : register(t2);
SamplerState sam : register(s2);

Texture2D normal_texture : register(t3);
SamplerState normal_sampler : register(s3);

//DF_INPUT ps(PS_INPUT input)
//{
//    DF_INPUT output;
//    output.light_weight = float4(0.f, 0.f, 0.f, 1.f);
    
//    float3 wp = world_position_texture.Sample(sam, input.texture_coordinate).rgb;
//    if (wp.r == 0.f && wp.g == 0.f && wp.b == 0.f)
//        return output;
//    float4 light_sv_pos = mul(float4(wp, 1.0f), cam_view_projection);
//    float2 proj_tex = float2(
//                light_sv_pos.x / light_sv_pos.w / 2.0f + 0.5f,
//                -light_sv_pos.y / light_sv_pos.w / 2.0f + 0.5f
//            );
//    float light_depth_value = distance(light_pos, wp);
//    float3 light_to_point_vec = normalize(wp - light_pos);
            
//    if ((saturate(proj_tex.x) != proj_tex.x) || (saturate(proj_tex.y) != proj_tex.y))
//        return output;
        
//    //////
//    // default output
//    //float depth_value = depth_texture.Sample(depth_sampler, proj_tex).r;
//    //if (light_depth_value < depth_value)
//    //{
//    //    float3 color = light_color * light_strength;
//    //    output.light_weight.rgb 
//    //        += color / (9.f * light_depth_value * light_depth_value) * a
//    //        + color / light_depth_value * b;
//    //}
//    //////
    
//    uint width2, height2, level2;
//    depth_texture.GetDimensions(0, width2, height2, level2);
//    float tex_x2 = 1.f / width2, tex_y2 = 1.f / height2;
    
//    float3 normal = normal_texture.Sample(normal_sampler, input.texture_coordinate);
//    float l_dot = dot(normal, -light_to_point_vec);
//    float3 bias_set = -light_to_point_vec - normal * l_dot;
    
//    if (l_dot <= 0.f)
//        return output;
    
//    //////
//    // sample output
//    float weight_sum = 0.f;
    
//    float depth_value = depth_texture.Sample(sam, proj_tex).r;
//    if (light_depth_value - bias_set.x - bias_set.y - bias_set.z <= depth_value)
//    {
//        float3 color = light_color * light_strength;
//        output.light_weight.rgb 
//            += color / (9.f * light_depth_value * light_depth_value) * a
//            + color / light_depth_value * b;
//        output.light_weight.rgb 
//            *= l_dot;
//    }
//    else
//    {
//        //float lu = depth_texture.Sample(sam, proj_tex + float2(-0.5f, -0.5f)).r;
//        //float ru = depth_texture.Sample(sam, proj_tex + float2(0.5f, -0.5f)).r;
//        //float ld = depth_texture.Sample(sam, proj_tex + float2(-0.5f, 0.5f)).r;
//        //float rd = depth_texture.Sample(sam, proj_tex + float2(0.5f, 0.5f)).r;
//    }
//    output.
//        light_weight.rgb += ambient_color * ambient_strength;
//    //////
    
//    return output;
//}

DF_INPUT ps(PS_INPUT input)
{
    float bias = 0.01f;
    
    DF_INPUT output;
    output.light_weight = float4(0.f, 0.f, 0.f, 1.f);
    
    float3 wp = world_position_texture.Sample(sam, input.texture_coordinate).rgb;
    if (wp.r == 0.f && wp.g == 0.f && wp.b == 0.f)
        return output;
    
    uint width2, height2, level2;
    depth_texture.GetDimensions(0, width2, height2, level2);
    float tex_x2 = 1.f / width2, tex_y2 = 1.f / height2;
    
    // Sample ing        
    float4 light_sv_pos = mul(float4(wp, 1.0f), cam_view_projection);
    float2 proj_tex = float2(
                light_sv_pos.x / light_sv_pos.w / 2.0f + 0.5f,
                -light_sv_pos.y / light_sv_pos.w / 2.0f + 0.5f
            );
    float light_depth_value = distance(light_pos, wp) - bias;
            
    if ((saturate(proj_tex.x) != proj_tex.x) || (saturate(proj_tex.y) != proj_tex.y))
        return output;
        
    int num_of_rect = 0;
    for (float x2 = -1.5f; x2 <= 1.5f; x2 += 1.0f)
        for (float y2 = -1.5f; y2 <= 1.5f; y2 += 1.0f)
        {
            float depth_value = depth_texture.Sample(sam, float2(proj_tex.x + x2 * tex_x2, proj_tex.y + y2 * tex_y2)).r;
        
            if (light_depth_value < depth_value)
            {
                num_of_rect++;
            }
        }
    
    float3 color = light_color * light_strength;
    if (num_of_rect > 0)
    {
        float3 normal = normal_texture.Sample(normal_sampler, input.texture_coordinate).rgb;
        
        float dot_r = dot(normalize(light_pos - wp), normal);
        if (dot_r > 0)
        {
            output.light_weight.rgb 
            += color / (9.f * light_depth_value * light_depth_value) * a
            + color / light_depth_value * b;
            
            output.light_weight.rgb *= dot_r;
            output.light_weight.rgb *= float(num_of_rect) / 16.f;
        }
    }
    output.light_weight.rgb += ambient_color * ambient_strength;
    
    return output;
}

//// 픽셀별로 라이팅을 처리하는데...
//DF_INPUT ps(PS_INPUT input)
//{
//    float bias = 0.001f;
    
//    DF_INPUT output;
//    output.light_weight = float4(0.f, 0.f, 0.f, 1.f);
    
//    float3 wp = world_position_texture.Sample(world_position_sampler, input.texture_coordinate).xyz;

//    if (wp.x == 0.f && wp.y == 0.f && wp.z == 0.f)
//    {
//        return output;
//    }
//    else
//    {
//        // 라이팅을 구하도록 한다.
//        float4 light_sv_pos = mul(float4(wp, 1.0f), cam_view_projection);
//        float2 proj_tex;
//        proj_tex.x = light_sv_pos.x / light_sv_pos.w / 2.0f + 0.5f;
//        proj_tex.y = -light_sv_pos.y / light_sv_pos.w / 2.0f + 0.5f;
//        float light_depth_value = distance(light_pos, wp);
    
//        if ((saturate(proj_tex.x) == proj_tex.x) &&
//            (saturate(proj_tex.y) == proj_tex.y))
//        {
//            #define LightMode 1
//            #if LightMode == 0
            
//            float2 texcoord = proj_tex;
//            float depth_value = depth_texture.Sample(g_samLinear, texcoord).r;
//            light_depth_value -= bias; // * light_depth_value;
        
//            if (light_depth_value < depth_value)
//            {
//                output.light_weight.rgb += light_color * light_strength / light_depth_value;
//            }
            
//            #endif
//            #if LightMode == 1
            
//            int num = 0;
//            uint width, height, level;
//            depth_texture.GetDimensions(0, width, height, level);
//            uint pix_x = proj_tex.x * width, pix_y = proj_tex.y * height;

//            for (float x = - 1.5f; x <= 1.5f; x += 1.f)
//                for (float y = - -1.5f; y <= 1.5f; y += 1.f)
//                {
//                    float2 texcoord = float2(x / width, y / height) + proj_tex;
//                    float depth_value = depth_texture.Sample(g_samLinear, texcoord).r;
//                    light_depth_value -= bias; // * light_depth_value;
        
//                    if (light_depth_value < depth_value)
//                    {
//                        output.light_weight.rgb += light_color * light_strength / light_depth_value;
//                        num++;
//                    }
                    
//                }
//            if (num != 0)
//                output.light_weight.rgb /= max(num, 4);
            
//            #endif
//            #if LightMode == 2
            
            
            
//            #endif
//        }
        
//    }
//    return output;
//}