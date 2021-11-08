#include "_Animation.h"
#include "../../Engine.h"

using namespace std;

ANIMATION::Animation::Animation()
{
}

void ANIMATION::Animation::draw_detail_view()
{
    Resource::draw_detail_view();

    if (ImGui::CollapsingHeader("Animation"))
    {
        if (ImGui::TreeNode("Key frame"))
        {
            ImGui::InputFloat(
                "Duration",
                &duration
            );

            ImGui::InputFloat(
                "Tick per sec",
                &tick_per_sec
            );
            
            if (ImGui::TreeNode("Key frame"))
            {
                for (size_t i = 0; i < key_frames.size(); ++i)
                {
                    char bone_name[200]{};
                    strcpy_s(bone_name, key_frames[i].bone_name.size() + 1, key_frames[i].bone_name.c_str());
                    ImGui::InputText("Bone name", bone_name, 100);

                    if (ImGui::TreeNode(
                        ("Scales##" + to_string(i)).c_str()
                    ))
                    {
                        for (size_t j = 0; j < key_frames[i].scales.size(); ++j)
                            if (ImGui::TreeNode(to_string(j).c_str()))
                            {
                                ImGui::InputFloat(
                                    ("TIme##S" + to_string(j)).c_str(),
                                    &key_frames[i].scales[j].time
                                );
                                ImGui::InputFloat3(
                                    ("Scale##S" + to_string(j)).c_str(), 
                                    &key_frames[i].scales[j].scale.x
                                );
                                ImGui::TreePop();
                            }
                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode(
                        ("Rotations" + to_string(i)).c_str()
                    ))
                    {
                        for (size_t j = 0; j < key_frames[i].rotations.size(); ++j)
                            if (ImGui::TreeNode(to_string(j).c_str()))
                            {
                                ImGui::InputFloat(
                                    ("TIme##R" + to_string(j)).c_str(), 
                                    &key_frames[i].rotations[j].time
                                );
                                ImGui::InputFloat4(
                                    ("Rotation##R" + to_string(j)).c_str(), 
                                    &key_frames[i].rotations[j].rot.x
                                );
                                ImGui::TreePop();
                            }
                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode(
                        ("Positions" + to_string(i)).c_str()
                    ))
                    {
                        for (size_t j = 0; j < key_frames[i].positions.size(); ++j)
                            if (ImGui::TreeNode(to_string(j).c_str()))
                            {
                                ImGui::InputFloat(
                                    ("TIme##P" + to_string(j)).c_str(), 
                                    &key_frames[i].positions[j].time
                                );
                                ImGui::InputFloat3(
                                    ("Position##P" + to_string(j)).c_str(), 
                                    &key_frames[i].positions[j].pos.x
                                );
                                ImGui::TreePop();
                            }
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        //for (size_t i = 0; i < Key_frame_transform.size(); ++i)
        //{
        //    if (ImGui::TreeNode((to_string(i) + " frame").c_str()))
        //    {
        //        for (size_t j = 0; j < Key_frame_transform[i].size(); ++j)
        //        {
        //            if (ImGui::TreeNode((to_string(j) + "bone").c_str()))
        //            {
        //                ImGui::TreePop();
        //            }
        //        }
        //        ImGui::TreePop();
        //    }
        //}

        // make clip
        if (ImGui::TreeNode("Work with animation"))
        {
            if (ImGui::Button((string("make clip##") + StringHelper::ptr_to_string(this)).c_str()))
            {
                unique_ptr<Clip> clip = make_unique<Clip>(this);

                auto shared = EDITOR::Shared::get_shared();
                shared->engine->resources.add_resource(
                    Name(&shared->engine->name_container, this->Registed_name.get_string() + "Clip"),
                    clip
                );
            }

            ImGui::TreePop();
        }
    }
}

void ANIMATION::Animation::load_resource(Engine* engine)
{

}

void ANIMATION::Animation::release_resource(Engine* engine)
{

}

//////
// Clip

ANIMATION::Clip::Clip()
{
}
ANIMATION::Clip::Clip(SafePtr<ANIMATION::Animation> animation)
{
    this->duration = animation->duration;
    this->tick_per_sec = animation->tick_per_sec;

    // bone 이름 저장하기
    int bone_index = 0;
    for (auto& iter : animation->key_frames)
    {
        bone_index_map[iter.bone_name] = bone_index;

        ++bone_index;
    }

    // bone transform의 크기 설정
    this->bone_transforms.resize(animation->key_frames.size());
    int array_size = static_cast<int>(this->duration);
    for (size_t i = 0; i < bone_transforms.size(); ++i)
        this->bone_transforms[i].resize(array_size);

    for (size_t frame_index = 0; frame_index < animation->key_frames.size(); ++frame_index)
    {
        auto& key_frame = animation->key_frames[frame_index];

        map<float, XMMATRIX> info;
        // 
        {
            for (auto& sca : key_frame.scales)
            {
                //iter = info.find(sca.time);
                //if (iter != info.end())
                //{
                //}
                //else
                {
                    info[sca.time] = XMMatrixScaling(sca.scale.x, sca.scale.y, sca.scale.z);
                }
            }
            for (auto& rot : key_frame.rotations)
            {
                XMVECTOR quat_vec;
                quat_vec.m128_f32[0] = rot.rot.x;
                quat_vec.m128_f32[1] = rot.rot.y;
                quat_vec.m128_f32[2] = rot.rot.z;
                quat_vec.m128_f32[3] = rot.rot.w;

                if (info.find(rot.time) != info.end())
                {
                    info[rot.time] *= XMMatrixRotationQuaternion(quat_vec);
                }
                else
                {
                    info[rot.time] = XMMatrixRotationQuaternion(quat_vec);
                }
            }
            for (auto& pos : key_frame.positions)
            {
                if (info.find(pos.time) != info.end())
                {
                    info[pos.time] *= XMMatrixTranslation(pos.pos.x, pos.pos.y, pos.pos.z);
                }
                else
                {
                    info[pos.time] = XMMatrixTranslation(pos.pos.x, pos.pos.y, pos.pos.z);
                }
            }
        }

        // 본 트렌스폼 설정
        auto& transforms = bone_transforms[frame_index];
        {
            auto front_iter = info.begin();
            auto back_iter = front_iter++;
            for (float t = 0.f; t < array_size; ++t)
            {
                // t가 끝에 도달 했을 때 다음 프레임 간격을 구한다.
                if (t > back_iter->first)
                {
                    front_iter = back_iter;
                    back_iter = ++back_iter;
                }

                float back_weight 
                    = (t - front_iter->first) 
                    / (back_iter->first - front_iter->first);

                transforms[static_cast<int>(t)]
                    = front_iter->second
                    * (1.f - back_weight)
                    + back_iter->second
                    * back_weight;
            }
        }

        // 오래된 코드
        {
            //// 트랜스폼 벡터 만들기.
            //this->bone_transforms[frame_index].resize(info.size());
            //int transform_index = 0;
            //for (auto& map_iter : info)
            //{
            //    bone_transforms[frame_index][transform_index]
            //        = map_iter.second;

            //    ++transform_index;
            //}

            //// 웨이트 필드 만들기
            //int index = 0;
            //int iter_index = 0;
            //auto front = info.begin();
            //auto end = ++info.begin();
            //while (end != info.end())
            //{
            //    this->frame_infos[frame_index][index].Front_index = iter_index;
            //    this->frame_infos[frame_index][index].End_index = iter_index + 1;
            //    //if (index == 0)
            //    //    this->frame_infos[frame_index][index].Lerp_amount = 0.0f;
            //    //else
            //    this->frame_infos[frame_index][index].Lerp_amount
            //        = ((float)index - front->first) / (end->first - front->first);

            //    // 현재 프레임 인포의 타임이 다음 인덱스보다 크다면 다음단계로 넘어가야 한다.
            //    ++index;
            //    if (index >= end->first)
            //    {
            //        front = end;
            //        ++end;
            //        ++iter_index;
            //    }
            //}
            //// 마지막 추가.
            //this->frame_infos[frame_index][index].Front_index = iter_index;
            //this->frame_infos[frame_index][index].End_index = iter_index;
            //this->frame_infos[frame_index][index].Lerp_amount = 0.0f;
        }
    }
}
void ANIMATION::Clip::draw_detail_view()
{
    Resource::draw_detail_view();

    if (ImGui::CollapsingHeader("Clip"))
    {
        std::string is_roop_name = "Is roop##" + StringHelper::ptr_to_string(&is_roop);
        ImGui::Checkbox(is_roop_name.c_str(), &is_roop);

        ImGui::InputFloat(
            string("Duration##" + StringHelper::ptr_to_string(&duration)).c_str(),
            &duration
        );
        ImGui::InputFloat(
            string("Tick per second##" + StringHelper::ptr_to_string(&tick_per_sec)).c_str(),
            &tick_per_sec
        );

        if (ImGui::TreeNode(("Bone transform##" + StringHelper::ptr_to_string(&bone_transforms)).c_str()))
        {
            for (size_t i = 0; i < bone_transforms.size(); ++i)
            {

                ImGui::SimpleTree(
                    to_string(i) + "##" + StringHelper::ptr_to_string(&bone_transforms),
                    [&]()
                    {
                        ImGui::vector_field<XMMATRIX>(
                            bone_transforms[i],
                            [&](size_t index, XMMATRIX& mat)
                            {
                                ImGui::SimpleTree(
                                    to_string(index) + "##" + StringHelper::ptr_to_string(&mat),
                                    [&]()
                                    {
                                        ImGui::XMMATRIX_field(mat, "Transform##" + to_string(i) + "_" + to_string(index));
                                    }
                                );
                            });
                    }
                );
            }
            ImGui::TreePop();
        }

        //if (ImGui::TreeNode("Frame info"))
        //{
        //    for (size_t i = 0; i < frame_infos.size(); ++i)
        //    {
        //        if (ImGui::TreeNode(
        //            (to_string(i) + "##" + StringHelper::ptr_to_string(&frame_infos[i])).c_str()
        //        ))
        //        {
        //            for (size_t j = 0; j < frame_infos[i].size(); ++j)
        //            {
        //                if (ImGui::TreeNode(
        //                    (to_string(j) + "##" + StringHelper::ptr_to_string(&frame_infos)).c_str()
        //                ))
        //                {
        //                    ImGui::InputFloat(
        //                        string("Lerp amount##" + StringHelper::ptr_to_string(&frame_infos[i][j].Lerp_amount)).c_str(),
        //                        &frame_infos[i][j].Lerp_amount
        //                    );
        //                    ImGui::InputInt(
        //                        string("Front##" + StringHelper::ptr_to_string(&frame_infos[i][j].Front_index)).c_str(),
        //                        &frame_infos[i][j].Front_index
        //                    );
        //                    ImGui::InputInt(
        //                        string("End##" + StringHelper::ptr_to_string(&frame_infos[i][j].End_index)).c_str(),
        //                        &frame_infos[i][j].End_index
        //                    );

        //                    ImGui::TreePop();
        //                }
        //            }

        //            ImGui::TreePop();
        //        }
        //    }

        //    ImGui::TreePop();
        //}

        for (size_t i = 0; i < bone_transforms.size(); ++i)
        {

        }
        for (size_t i = 0; i < bone_transforms.size(); ++i)
        {

        }
    }
}

void ANIMATION::Clip::get_lerp(float& now_time, int frame_index, XMMATRIX* des)
{
    int front, back;
    float lerp;

    if (now_time < 0.f) now_time = 0.f;

    if (is_roop)
    {
        // 현재 시간이 듀레이션보다 클경우 duration으로 warp함으로써
        //if문에서 duration보다 큰 경우를 배제함.
        // 
        // now_time == duration의 경우 now_time은 0으로 설정됨.
        // now_time == duration + 0.5의 경우 now_time은 0.5으로 설정됨.
        // now_time == duration + 1의 경우 now_time은 1으로 설정됨.
        if (now_time > duration)
            now_time = now_time - static_cast<int>(now_time / duration) * duration;

        if (duration - 1.f > now_time)
        {
            front = static_cast<int>(now_time);
            back = front + 1;
            lerp = now_time - front;
        }
        else if (duration == now_time - 1.f)
        {
            front = static_cast<int>(now_time);
            back = 0;
            lerp = 0.f;
        }
        else if (duration > now_time)
        {
            front = static_cast<int>(now_time);
            back = 0;
            lerp = now_time - front;
        }
        else if (duration == now_time)
        {
            front = static_cast<int>(now_time) - 1;
            back = 0;
            lerp = 1.f;
            now_time = 0.f;
        }
    }
    else
    {
        // roop가 아닐때는 now_time이 초과했을 때 끝으로 간다.
        if (duration - 1.f > now_time)
        {
            front = static_cast<int>(now_time);
            back = front + 1;
            lerp = now_time - front;
        }
        else if (duration == now_time - 1.f)
        {
            front = static_cast<int>(now_time);
            back = front;
            lerp = 0.f;
        }
        else if (duration > now_time)
        {
            front = static_cast<int>(now_time);
            back = front;
            lerp = now_time - front;
        }
        else if (duration == now_time)
        {
            front = static_cast<int>(now_time) - 1;
            back = front;
            lerp = 1.f;
        }
        else if (now_time > duration)
        {
            now_time == duration;
            front = bone_transforms.size() - 1;
            back = front;
            lerp = 1.f;
        }
    }

#pragma message(__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: 듀레이션과 tick per sec을 배제함.")
    *des
        = bone_transforms[frame_index][front]
        * (1.f - lerp)
        + bone_transforms[frame_index][back]
        * lerp
        ; // end
}
void ANIMATION::Clip::get_lerp(float& now_time, std::string& bone_name, XMMATRIX* des)
{
    get_lerp(now_time, bone_index_map[bone_name], des);
}

void ANIMATION::Clip::load_resource(Engine* engine)
{

}

void ANIMATION::Clip::release_resource(Engine* engine)
{

}

//////
// Mapping

void ANIMATION::Mapping::load_resource(Engine* engine)
{

}

void ANIMATION::Mapping::release_resource(Engine* engine)
{

}