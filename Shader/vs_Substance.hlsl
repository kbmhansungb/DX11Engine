#pragma pack_matrix(row_major)
#include "_VS_INPUT_0_VERTEX.fx"
#include "_VS_INPUT_1_SUBSTANCE.fx"
#include "_VS_INPUT_2_BONE_ID_AND_WEIGHTS.fx"
#include "_PS_INPUT.fx"

#include "_CONSTANT_1_VIEW_PROJECTION.fx"
#include "_CONSTANT_2_BONE_WORLDS.fx"

PS_INPUT vs(
VS_INPUT_VERTEX vertex, 
VS_INPUT_SUBSTANCE substance, 
VS_INPUT_BONE_ID_AND_WEIGHTS bone_id_weights
)
{
    PS_INPUT output;
    
    output.texture_coordinate = vertex.texture_coordinate;
    output.vertex_color_0 = substance.vertex_color;
    
    matrix mat = get_weighted_bone_matrix(bone_id_weights.bone_ID, bone_id_weights.weight);
    
    output.normal = normalize(mul(vertex.normal, (float3x3) mat));
    output.world_position = mul(float4(vertex.position, 1.0f), mat);
    
    mat = mul(mat, ViewProjection);
    
    output.position = mul(float4(vertex.position, 1),mat);
    
    return output;
}