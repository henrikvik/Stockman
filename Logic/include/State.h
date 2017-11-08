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
    class StatePrimary;
    class StateSecondary;
    struct StateBuffer;
    class State : public NonCopyable
    {
    public:
        State(StateBuffer* stateBuffer) : m_stateBuffer(stateBuffer) { }
        virtual ~State() { }
        virtual void reset() = 0;
        virtual void update(float deltaTime) = 0;
        virtual void render() const = 0;

        inline void SetFuncPrimarySwitch(std::function<void(StateType)> gameSwitch)         { SwitchPrimaryState       = gameSwitch;  }
        inline void SetFuncSecondarySwitch(std::function<void(StateType)> menuSwitch)       { SwitchSecondaryState     = menuSwitch;  }
        inline void SetFuncGetCurrentPrimary(std::function<StatePrimary*()> gameState)      { GetCurrentPrimaryState   = gameState;   }
        inline void SetFuncGetCurrentSecondary(std::function<StateSecondary*()> menuState)  { GetCurrentSecondaryState = menuState;   }

    protected:
        StateBuffer* m_stateBuffer;
        std::function<void(StateType)> SwitchSecondaryState;
        std::function<void(StateType)> SwitchPrimaryState;
        std::function<StatePrimary*()> GetCurrentPrimaryState;
        std::function<StateSecondary*()> GetCurrentSecondaryState;
    };
}

#endif // !STATE_H
