#ifndef STATE_H
#define STATE_H

// Include DirectX to get Math Vectors
#include <d3d11.h>
#include <SimpleMath.h>
#include <Misc\NonCopyable.h>   
#include <cstdio>               // <3 printf()    
#include <GameType.h>

namespace Graphics { class Renderer; }
namespace Logic
{
    class StateMachine;
    class State : public NonCopyable
    {
    public:
        virtual ~State() { }
        virtual void reset() = 0;
        virtual void update(float deltaTime) = 0;
        virtual void render(Graphics::Renderer& renderer) = 0;

        virtual DirectX::SimpleMath::Vector3 getCameraForward() = 0;
        virtual DirectX::SimpleMath::Vector3 getCameraPosition() = 0;

        void setCallBackFunction(std::function<void(StateType)> switchState)
        {
            this->SetState = switchState;
        }

    protected:
        // This state will be broken after use, so only use at the end of update()
        std::function<void(StateType)> SetState;
    };
}

#endif // !STATE_H
