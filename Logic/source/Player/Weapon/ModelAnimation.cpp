#include <Player\Weapon\ModelAnimation.h>

using namespace Logic;

#define ANIMATION_SPEED 0.020f

ModelAnimation::ModelAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame)
{
    m_frameActive   = defaultFrame;
    m_frameDefault  = defaultFrame;
    m_frameTarget   = defaultFrame;
    m_model.model   = modelID;
    m_animation     = Animation();
}

ModelAnimation::~ModelAnimation()
{

}

void ModelAnimation::addAnimation(float totalDuration, std::queue<AnimationFrame> frames)
{
    if (m_animation.initialize(totalDuration, frames))
    {
        printf("Started an animation for %f seconds with %d frames.\n", totalDuration, frames.size());
    }
    else
    {
        printf("Could not start animation.\n");
    }
}

// \param dt The delta time
// \param offsetTranslation The translation matrix to place this model
// \param offsetForward In which direction this model should be placed
void ModelAnimation::update(float dt, DirectX::SimpleMath::Matrix offsetTranslation, DirectX::SimpleMath::Vector3 offsetForward)
{
    // Multiplying all matrises to place the model correctly
    DirectX::SimpleMath::Matrix cameraMatrix = DirectX::XMMatrixLookToRH({ 0, 0, 0 }, offsetForward, { 0, 1, 0 });
    DirectX::SimpleMath::Matrix offsetMatrix = (DirectX::SimpleMath::Matrix::CreateTranslation(offsetTranslation.Translation() + offsetForward));
    DirectX::SimpleMath::Matrix resultMatrix = m_frameActive.rotation * m_frameActive.translation * m_frameActive.scale * cameraMatrix.Invert() * offsetMatrix;
    m_model.transform = resultMatrix;

    // Updates this model's animation and sets the targeted frame
    if (!m_animation.done)  m_animation.update(dt);
    if (!m_animation.done)  m_frameTarget = m_animation.getCurrentFrame();
    else                    m_frameTarget = m_frameDefault;

    // Easing to the targeted frame
    m_frameActive.easeTo(dt, m_frameTarget);
}

void ModelAnimation::render() const
{
    QueueRender(m_model);
}


/*************************
* Animation Frame Struct *
*************************/

// Eases this animationframe to another
void ModelAnimation::AnimationFrame::easeTo(float dt, const AnimationFrame & other)
{
    rotation    += (other.rotation - rotation)          * ANIMATION_SPEED * dt;
    translation += (other.translation - translation)    * ANIMATION_SPEED * dt;
    scale       += (other.scale - scale)                * ANIMATION_SPEED * dt;
}

/*************************
* Animation Struct       *
*************************/

// Basic Constructor for animation objects
ModelAnimation::Animation::Animation()
{
    done                = true;
    timer               = 0.f;
    timeBetweenFrames   = 0.f;
}

// Initializer for an animation
bool ModelAnimation::Animation::initialize(float totalTime, std::queue<AnimationFrame> frames)
{
    if (!frames.empty())
    {
        done = false;
        timer = 0.f;
        timeBetweenFrames = totalTime / frames.size();
        return true;
    }
    else
    {
        done = true;
        return false;
    }
}

// Tries to switch frame and checks if completed
// \note Can only be called if "done" is false
void ModelAnimation::Animation::update(float dt)
{
    timer += dt;
    if (timer > timeBetweenFrames)
    {
        printf("Frame completed, popped.\n");
        frames.pop();
        timer = 0.f;

        if (frames.empty())
            done = true;
    }
}

// \note Can only be called if "done" is false
ModelAnimation::AnimationFrame ModelAnimation::Animation::getCurrentFrame()
{
    return frames.front();
}

/*************************
* Weapon Animation       *
*************************/

WeaponAnimation::WeaponAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame)
 : ModelAnimation(modelID, defaultFrame) { }

void WeaponAnimation::startReloadingAnimation(float reloadTime)
{
    std::queue<AnimationFrame> frames;
   
    AnimationFrame frame = m_frameDefault;
    frame.translation._42 += 1;
    frames.push(frame);

    addAnimation(reloadTime, frames);
}