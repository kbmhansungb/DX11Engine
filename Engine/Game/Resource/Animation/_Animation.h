#pragma once

#include "../Resource.h"
#include "../Skeleton/_Exclude.h"

namespace ANIMATION
{
    class Animation : public Resource
    {
    public:
        struct keyScale
        {
            float time;
            XMFLOAT3 scale;
        };

        struct keyPosition
        {
            float time;
            XMFLOAT3 pos;
        };

        struct keyRotation
        {
            float time;
            XMFLOAT4 rot;
        };
        struct KeyFrame
        {
            std::string bone_name;
            std::vector<keyScale> scales;
            std::vector<keyRotation> rotations;
            std::vector<keyPosition> positions;
        };
        std::vector<KeyFrame> key_frames;
        float duration = -1.0f;
        float tick_per_sec = -1.0f;

        int max_key_frame = -1;
        float animation_speed = -1.0f;

        Animation();
        virtual void draw_detail_view() override;

        virtual void load_resource(Engine* engine) override;
        virtual void release_resource(Engine* engine) override;
    };

    ///
    /// 참고 : clip에서의 순서는 스켈레톤의 순서와 다를 수 있다. 맵에서 찾아서 인덱스를 구해야 할 수 있다.
    /// Frame info가 불필요하단 사실을 깨닫고 파기함.
    class Clip : public Resource
    {
    public:
        //class FrameInfo
        //{
        //public:
        //    int Front_index;
        //    int End_index;
        //    // Lerp_amount == 1 >>> End transform
        //    float Lerp_amount;
        //};
        std::unordered_map<std::string, int> bone_index_map;

        bool is_roop = false;
        float duration = -1.0f;
        float tick_per_sec = -1.0f;

        std::vector<std::vector<XMMATRIX>> bone_transforms;
        //std::vector<std::vector<FrameInfo>> frame_infos;

        Clip();
        Clip(SafePtr<ANIMATION::Animation> animation);
        virtual void draw_detail_view();

        void get_lerp(float& now_time, int frame_index, XMMATRIX* des);
        void get_lerp(float& now_time, std::string& bone_name, XMMATRIX* des);

        virtual void load_resource(Engine* engine) override;
        virtual void release_resource(Engine* engine) override;
    };

    class Mapping : public Resource
    {
    public:
        std::vector<int> clip_index;

        virtual void load_resource(Engine* engine) override;
        virtual void release_resource(Engine* engine) override;
    };
}