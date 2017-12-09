#include <Entity/AnimatedModel.h>
#include <Graphics\include\RenderQueue.h>

AnimatedModel::AnimatedModel(Resources::Models::Files model_id, const char * start_animation)
{
    auto model = ModelLoader::get().getModel(model_id);
    skeleton = &model->getSkeleton();

    render_info.model            = model_id;
    render_info.color            = Vector3(1,1,1);
    render_info.cull_radius      = model->get_bounding_box().sphere_radius();
    render_info.joint_transforms = &joint_transforms;
    render_info.useGridTexture   = false;

    frame_progress = 0.0f;
    frame_duration = 1.0f / 60.0f;
    delta_multiplier = 1.f;

    animation_progress = 0.0f;
    animation_duration = 0.0f;
    animation_current  = "";
    animation_next     = start_animation;

    callback_next    = nullptr;  
}

void AnimatedModel::update(float delta_ms)
{
    static float to_seconds = 1.0f / 1000.0f;
    frame_progress += delta_ms * delta_multiplier * to_seconds;

    // early return if its not time to update the animation
    if (frame_progress < frame_duration) return;

    frame_progress     -= frame_duration;
    animation_progress += frame_duration;

    // check if there is an animation to update
    if (!animation_current.empty())
    { 
        joint_transforms = skeleton->evalAnimation(
            animation_current.c_str(),
            animation_progress
        );
    }

    // early return if the animation has not finished
    if (animation_progress < animation_duration) return;

    frame_progress     = 0.0f;
    animation_progress = 0.0f;

    // early return if there is no animation queued
    if (animation_next.empty()) return;

    animation_current  = animation_next;
    animation_duration = skeleton->getAnimationDuration(animation_next.c_str());    
    animation_next     = "";

    // early return if the animation queded had no callback
    if (callback_next == nullptr) return;

    callback_next();
}

void AnimatedModel::render() const
{
    QueueRender(render_info);
}

void AnimatedModel::set_color(DirectX::SimpleMath::Vector3 const &color)
{
    render_info.color = color;
}

void AnimatedModel::set_next(const char *animation, std::function<void(void)> callback)
{
    animation_next = animation;
    callback_next  = callback;
}

void AnimatedModel::set_transform(DirectX::SimpleMath::Matrix &transform)
{
    render_info.transform = transform;
}

void AnimatedModel::set_delta_multiplier(float multiplier)
{
    this->delta_multiplier = multiplier;
}

float AnimatedModel::get_animation_time() const
{
    return animation_duration;
}
