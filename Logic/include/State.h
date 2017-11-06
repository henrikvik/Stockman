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
    class StateGame;
    class StateMenu;
    class State : public NonCopyable
    {
    public:
        virtual ~State() { }
        virtual void reset() = 0;
        virtual void update(float deltaTime) = 0;
        virtual void render() const = 0;

        inline void SetGameSwitchCallBack(std::function<void(StateType)> switchState)  { SwitchParentGameState        = switchState;  }
        inline void SetMenuSwitchCallBack(std::function<void(StateType)> switchState)  { SwitchParentMenuState        = switchState;  }
        inline void SetCurrentGameState(std::function<StateGame*()> gameState)         { GetParentCurrentGameState    = gameState;    }
        inline void SetCurrentMenuState(std::function<StateMenu*()> menuState)         { GetParentCurrentMenuState    = menuState;    }

    protected:
        std::function<void(StateType)> SwitchParentMenuState;
        std::function<void(StateType)> SwitchParentGameState;
        std::function<StateGame*()> GetParentCurrentGameState;
        std::function<StateMenu*()> GetParentCurrentMenuState;
    };
}

#endif // !STATE_H
