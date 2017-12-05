#include <Player\Weapon\ModelAnimation.h>

using namespace Logic;

#define ANIMATION_SPEED                 0.020f
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
bool ModelAnimation::Animation::initialize(float totalTime, std::queue<AnimationFrame> inFrames)
{
    if (!inFrames.empty())
    {
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

/*************************
* Weapon Animation       *
*************************/

WeaponAnimation::WeaponAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame)
 : ModelAnimation(modelID, defaultFrame) { }

void WeaponAnimation::startReloadingAnimation(float reloadTime)
{
    std::queue<AnimationFrame> frames;
   
    // Just move the inwards to the player model, outside view
    AnimationFrame frame = m_frameDefault;
    frame.translation._42 -= 0.75f;
    frame.translation._43 += 0.75;
    frames.push(frame);

    addAnimation(reloadTime, frames);
}

void WeaponAnimation::startSwapToAnimation(float swaptimer)
{
    // Forcing the current position a spot below the player
    m_frameActive = m_frameDefault;
    m_frameActive.translation._42 = m_frameDefault.translation._42 - 0.50f;
    m_frameActive.translation._43 = m_frameDefault.translation._43 + 0.50f;
}

void WeaponAnimation::startWindupAnimation(float delayTimer)
{
    std::queue<AnimationFrame> frames;

    AnimationFrame frame = m_frameDefault;

    // Moving backwards and to the right
    frame.translation._41 += 1.0f;
    frame.translation._43 += 0.75f;

    // Rotating 45 degrees 
    DirectX::SimpleMath::Matrix rotationMatrix = m_frameDefault.rotation;
    float angle = 45.f * 3.14 / 180.f;
    rotationMatrix._11 += cos(angle);
    rotationMatrix._12 += sin(angle);
    rotationMatrix._21 += -sin(angle);
    rotationMatrix._22 += cos(angle);
    frame.rotation = rotationMatrix;

    frames.push(frame);

    addAnimation(delayTimer, frames);
}

void WeaponAnimation::startShootAnimation(float attackTimer, bool primary)
{
    std::queue<AnimationFrame> frames;

    // Just move the inwards to the player model, outside view
    AnimationFrame frame = m_frameDefault;
    frame.translation._43 += 0.25f;
    frames.push(frame);

    addAnimation(attackTimer, frames);
}

/*************************
* Sledge Hammer Animation*
*************************/

WeaponSledgeHammerAnimation::WeaponSledgeHammerAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame)
    : WeaponAnimation(modelID, defaultFrame) { }

void WeaponSledgeHammerAnimation::startShootAnimation(float attackTimer, bool primary)
{
    std::queue<AnimationFrame> frames;
    float halfAttackTimer = attackTimer / 2.f;

    if (!primary)
    {
        // Parry
        AnimationFrame frame = m_frameDefault;
        frame.translation._41 -= 0.1f;
        frame.translation._42 += 0.5f;
        frame.translation._43 += 0.3f;
        frames.push(frame);
    }
    else
    {
        // Sledge First Frame
        AnimationFrame frame = m_frameDefault;
        frame.translation._41 += 0.6f;
        frame.translation._42 += 0.6f;
        frame.translation._43 += 1.25f;
        DirectX::SimpleMath::Matrix rotationMatrix;
        DirectX::SimpleMath::Matrix x = rotationMatrix.CreateRotationX((0.f * 3.14) / 180.f);
        DirectX::SimpleMath::Matrix y = rotationMatrix.CreateRotationY((105.f * 3.14) / 180.f);
        DirectX::SimpleMath::Matrix z = rotationMatrix.CreateRotationZ((287.f * 3.14) / 180.f);
        frame.rotation = z * y * x;
        frames.push(frame);
    }

    addAnimation(halfAttackTimer, frames);
}