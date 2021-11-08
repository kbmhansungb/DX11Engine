#define MAX_INSTANCE_SIZE 1000
cbuffer instance_worlds : register(b2)
{
    matrix worlds[MAX_INSTANCE_SIZE];
};