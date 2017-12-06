#include <Player\Weapon\ModelAnimation.h>

using namespace Logic;

#define PRINT_DEBUG                     false
#define ANIMATION_SPEED                 0.015f
#define HEIGHT_POSITION_OFFSET_EASING   0.038f

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
    // Popping all the current animation frames
    while (!m_animation.frames.empty())
        m_animation.frames.pop();
}

void ModelAnimation::addAnimation(float totalDuration, std::queue<AnimationFrame> frames)
{
    if (m_animation.initialize(totalDuration, frames))
    {
        #if PRINT_DEBUG
        printf("Started an animation for %f seconds with %d frames.\n", totalDuration, frames.size());
        #endif
    }
    else
    {
#       if PRINT_DEBUG
        printf("Could not start animation.\n");
        #endif
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
    
    // Easing the y position of the model
    float tempYPos = m_model.transform._42;
    m_model.transform = resultMatrix;
    m_model.transform._42 = tempYPos;
    m_model.transform._42 += (resultMatrix._42 - m_model.transform._42) * min((HEIGHT_POSITION_OFFSET_EASING / sqrt(m_frameActive.scale._22) * dt), 1.f);

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
    rotation    += (other.rotation - rotation)          * ANIMATION_SPEED * other.easingModifier * dt;
    translation += (other.translation - translation)    * ANIMATION_SPEED * other.easingModifier * dt;
    scale       += (other.scale - scale)                * ANIMATION_SPEED * other.easingModifier * dt;
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
bool ModelAnimation::Animation::initialize(float totalTime, std::queue<AnimationFrame> inFrames)
{
    if (!inFrames.empty())
    {
        while (!frames.empty())
            frames.pop();
        
        done = false;
        timer = 0.f;
        timeBetweenFrames = totalTime / inFrames.size();
        frames = inFrames;
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