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
        virtual void render() const = 0;

        void SetGameSwitchCallBack(std::function<void(StateType)> switchState) { SwitchGameState = switchState; }
        void SetMenuSwitchCallBack(std::function<void(StateType)> switchState) { SwitchMenuState = switchState; }

    protected:
        std::function<void(StateType)> SwitchMenuState;
        std::function<void(StateType)> SwitchGameState;
    };
}

#endif // !STATE_H
