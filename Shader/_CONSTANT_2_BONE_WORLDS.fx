#define MAX_BONE_LINK           8
#define MAX_BONE_SIZE           256

struct BoneMatrix
{
    matrix world;
    matrix world_inverse;
};
// c++에서 {{buff1}, {buff2}, {buff3}, ... }
// 이런식으로 넘어온다. 이렇게 하면 순서대로 정리되서 들어오는가?
cbuffer BoneMatrixs : register(b2)
{
    BoneMatrix BoneMatrixs[MAX_BONE_SIZE];
}

matrix get_weighted_bone_matrix(uint boneIds[MAX_BONE_LINK], float weights[MAX_BONE_LINK])
{
    matrix mat = 0;
    for (int i = 0; i < MAX_BONE_LINK; ++i)
    {
        if (weights[i] != 0.f)
            mat += mul(weights[i], BoneMatrixs[boneIds[i]].world);
    }
    //mat += mul(weights.x, BoneMatrixs[boneIds.x].world);
    //if (weights.y != 0)
    //    mat += mul(weights.y, BoneMatrixs[boneIds.y].world);
    //if (weights.z != 0)
    //    mat += mul(weights.z, BoneMatrixs[boneIds.z].world);
    //if (weights.w != 0)
    //    mat += mul(weights.w, BoneMatrixs[boneIds.w].world);
    return mat;
}