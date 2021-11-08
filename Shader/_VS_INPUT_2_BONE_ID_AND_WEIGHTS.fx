struct VS_INPUT_BONE_ID_AND_WEIGHTS
{
    //uint4 bone_ID : BONEIDS;
    uint bone_ID[8] : BONEIDS;
    //float4 weight : WEIGHTS;
    float weight[8] : WEIGHTS;
};