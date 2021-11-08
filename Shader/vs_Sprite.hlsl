#pragma pack_matrix(row_major)
#include "_VS_INPUT_0_VERTEX.fx"
#include "_PS_INPUT.fx"

PS_INPUT vs(VS_INPUT_VERTEX input)
{
    PS_INPUT output;
    output.position = float4(input.position, 1.0f);
    output.texture_coordinate = input.texture_coordinate;
    
	return output;
}