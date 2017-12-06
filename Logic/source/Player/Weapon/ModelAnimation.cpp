#include <Player\Weapon\ModelAnimation.h>

using namespace Logic;

#define PRINT_DEBUG                     false
#define EDIT_VARIABLES                  false
#define ANIMATION_SPEED                 0.015f
#define HEIGHT_POSITION_OFFSET_EASING   0.038f

#if EDIT_VARIABLES
    float var_1 = 0;
    float var_2 = 0;
    int var_3 = 0;
    float var_4 = 0.f;
    float rot_11 = 90;
    float rot_12 = 90;
    float rot_13 = 90;
    float rot_21 = 90;
    float rot_22 = 90;
    float rot_23 = 90;
    float rot_31 = 90;
    float rot_32 = 90;
    float rot_33 = 90;
#endif

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

#if EDIT_VARIABLES
    ImGui::Begin("Edit");
    ImGui::DragFloat("Pos", &var_1, 0.01f, 0.f, 20.f, "%.4f");
    ImGui::DragFloat("Easing Start", &var_4, 0.001f, 0.f, 1.0f, "%.6f");
    ImGui::DragFloat("Easing Mod", &var_2, 0.0001f, 0.f, 1.0f, "%.6f");
    ImGui::DragInt("Times", &var_3, 1.f, 0.f, 20.f, "%.0f");
    ImGui::DragFloat("LEFT Rot_11", &rot_11, 0.1f, 0.f, 360.f, "%.4f");
    ImGui::DragFloat("LEFT Rot_12", &rot_12, 0.1f, 0.f, 360.f, "%.4f");
    ImGui::DragFloat("LEFT Rot_13", &rot_13, 0.1f, 0.f, 360.f, "%.4f");
    ImGui::DragFloat("RIGHT Rot_21", &rot_21, 0.1f, 0.f, 360.f, "%.4f");
    ImGui::DragFloat("RIGHT Rot_22", &rot_22, 0.1f, 0.f, 360.f, "%.4f");
    ImGui::DragFloat("RIGHT Rot_23", &rot_23, 0.1f, 0.f, 360.f, "%.4f");
    ImGui::DragFloat("CLOSE Rot_31", &rot_31, 0.1f, 0.f, 360.f, "%.4f");
    ImGui::DragFloat("CLOSE Rot_32", &rot_32, 0.1f, 0.f, 360.f, "%.4f");
    ImGui::DragFloat("CLOSE Rot_33", &rot_33, 0.1f, 0.f, 360.f, "%.4f");
    ImGui::End();
#endif

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

/*************************
* Weapon Animation       *
*************************/

WeaponAnimation::WeaponAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame)
 : ModelAnimation(modelID, defaultFrame) 
{
    m_enhancedColorDefault  = DirectX::SimpleMath::Vector3(1, 0.6, 0.8);
    m_enhancedColorCurrent  = DirectX::SimpleMath::Vector3(1, 1, 1);
    m_enhancedColorTarget   = DirectX::SimpleMath::Vector3(1, 1, 1);
    m_isEnhanced            = false;
}

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

void WeaponAnimation::update(float dt, DirectX::SimpleMath::Matrix offsetTranslation, DirectX::SimpleMath::Vector3 offsetForward)
{
    // Enhanced Easing
    if (m_isEnhanced)
    {
        m_enhancedColorTarget     = m_enhancedColorDefault;
        m_enhancedScaleTarget._11 = 1.10f;
        m_enhancedScaleTarget._22 = 1.10f;
        m_enhancedScaleTarget._33 = 1.10f;
    }
    else
    {
        m_enhancedColorTarget     = DirectX::SimpleMath::Vector3(1, 1, 1);
        m_enhancedScaleTarget._11 = 1.f;
        m_enhancedScaleTarget._22 = 1.f;
        m_enhancedScaleTarget._33 = 1.f;
    }

    ModelAnimation::update(dt, offsetTranslation, offsetForward);

    // Enhanced Easing
    m_enhancedColorCurrent += (m_enhancedColorTarget - m_enhancedColorCurrent) * ANIMATION_SPEED * dt;
    m_enhancedScaleCurrent += (m_enhancedScaleTarget - m_enhancedScaleCurrent) * ANIMATION_SPEED * dt;
    m_model.color           = m_enhancedColorCurrent;
    m_model.transform       = m_enhancedScaleCurrent * m_model.transform;
}

/*************************
* Sledge Hammer Animation*
*************************/

WeaponSledgeHammerAnimation::WeaponSledgeHammerAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame)
    : WeaponAnimation(modelID, defaultFrame) 
{
    m_enhancedColorDefault = DirectX::SimpleMath::Vector3(0.6, 0.8, 1.f);
}

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
        // Sledge Hammer
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

/*************************
* Freeze Gun Animation   *
*************************/

WeaponFreezeGunAnimation::WeaponFreezeGunAnimation(Resources::Models::Files modelID, AnimationFrame defaultFrame)
    : WeaponAnimation(modelID, defaultFrame) 
{
    m_enhancedColorDefault = DirectX::SimpleMath::Vector3(0.6, 1.0, 0.7);
}

void WeaponFreezeGunAnimation::startShootAnimation(float attackTimer, bool primary)
{
    if (!primary)
    {
        std::queue<AnimationFrame> frames;

        // Freeze Bomb
        AnimationFrame frame = m_frameDefault;
        frame.translation._42 -= 0.20f;
        frame.translation._43 += 0.25f;
        frames.push(frame);

        addAnimation(attackTimer, frames);
    }
    else
    {
        // Freeze Spray
        std::queue<AnimationFrame> frames;

        AnimationFrame frame = m_frameDefault;
        frame.translation._42 += 0.13;
        frame.translation._43 += 0.09;
        DirectX::SimpleMath::Matrix rotationMatrix;
        DirectX::SimpleMath::Matrix x = rotationMatrix.CreateRotationX((344.8f * 3.14) / 180.f);
        DirectX::SimpleMath::Matrix y = rotationMatrix.CreateRotationY((27.f * 3.14) / 180.f);
        DirectX::SimpleMath::Matrix z = rotationMatrix.CreateRotationZ((0.0 * 3.14) / 180.f);
        frame.rotation = z * y * x;
        frames.push(frame);
        m_frameActive = frame;

        addAnimation(attackTimer, frames);
    }
}