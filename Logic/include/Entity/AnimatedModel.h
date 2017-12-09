#pragma once
#include <functional>
#include <Graphics\include\RenderInfo.h>
#include <Graphics\include\Utility\ModelLoader.h>
#include <btBulletDynamicsCommon.h>

class AnimatedModel
{
public:
    AnimatedModel(Resources::Models::Files model_id, const char * start_animation = "");

    void update(float delta_ms);
    void render() const;

    void set_color(DirectX::SimpleMath::Vector3 const &color);
    void set_next(const char * animation, std::function<void(void)> callback = nullptr);
    void set_transform(DirectX::SimpleMath::Matrix &transform);
    void set_delta_multiplier(float multiplier);

    float get_animation_time() const;

private:
    using Matrix = DirectX::SimpleMath::Matrix;
    using Vector3 = DirectX::SimpleMath::Vector3;

    NewAnimatedRenderInfo render_info;

    HybrisLoader::Skeleton * skeleton;
    std::vector<Matrix> joint_transforms;

    float frame_progress;
    float frame_duration;
    float delta_multiplier;

    float animation_progress;
    float animation_duration;

    std::string animation_current;
    std::string animation_next;

    std::function<void(void)> callback_next;
};