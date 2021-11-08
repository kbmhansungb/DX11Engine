#pragma pack_matrix(row_major)
#include "_VS_INPUT_0_VERTEX.fx"
#include "_PS_INPUT.fx"

#include "_CONSTANT_0_WORLD.fx"
#include "_CONSTANT_1_VIEW_PROJECTION.fx"

PS_INPUT vs(VS_INPUT_VERTEX vertex)
{
    PS_INPUT output;
    
    output.texture_coordinate = vertex.texture_coordinate;
    
    matrix mat = world;
    
    output.normal = normalize(mul(vertex.normal, (float3x3) mat));
    output.world_position = mul(float4(vertex.position, 1.0f), mat);
    
    mat = mul(mat, ViewProjection);
    
    output.position = mul(float4(vertex.position, 1), mat);
    
    return output;
}