#pragma pack_matrix(row_major)
#include "_VS_INPUT_0_VERTEX.fx"
#include "_VS_INPUT_1_TERRAIN.fx"
#include "_PS_INPUT.fx"

#include "_CONSTANT_0_WORLD.fx"
#include "_CONSTANT_1_VIEW_PROJECTION.fx"

PS_INPUT vs(VS_INPUT_VERTEX vertex, VS_INPUT_TERRAIN terrain)
{
    PS_INPUT output;
    
    output.texture_coordinate = vertex.texture_coordinate;
    
    matrix mat = world;
    
    output.normal = normalize(mul(vertex.normal, (float3x3) mat));
    output.world_position = mul(float4(vertex.position, 1.0f), mat);
    
    mat = mul(mat, ViewProjection);
    
    output.position = mul(float4(vertex.position, 1.0f), mat);
    
    // to texture weight
    for (int j = 0; j < 3; ++j)
    {
        if (terrain.weights[j] > 0.0f)
        {
            if (0 == terrain.indexs[j])
            {
                output.vertex_color_0.x = terrain.weights[j];
            }
            else if (1 == terrain.indexs[j])
            {
                output.vertex_color_0.y = terrain.weights[j];
            }
            else if (2 == terrain.indexs[j])
            {
                output.vertex_color_0.z = terrain.weights[j];
            }
            else if (3 == terrain.indexs[j])
            {
                output.vertex_color_0.w = terrain.weights[j];
            }
            else if (4 == terrain.indexs[j])
            {
                output.vertex_color_1.x = terrain.weights[j];
            }
            else if (5 == terrain.indexs[j])
            {
                output.vertex_color_1.y = terrain.weights[j];
            }
            else if (6 == terrain.indexs[j])
            {
                output.vertex_color_1.z = terrain.weights[j];
            }
            else if (7 == terrain.indexs[j])
            {
                output.vertex_color_1.w = terrain.weights[j];
            }
            else if (8 == terrain.indexs[j])
            {
                output.vertex_color_2.x = terrain.weights[j];
            }
        }
    }
    return output;
}