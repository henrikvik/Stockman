#ifndef STATEGAME_H
#define STATEGAME_H

#include <State.h>

namespace Logic
{
    class StateGame : public State
    {
    public:
        StateGame();
        ~StateGame();
        void reset();
        void update(float deltaTime);
        void render(Graphics::Renderer& renderer);
    };
}

#endif // !STATEGAME_H
