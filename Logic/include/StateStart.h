#ifndef STATESTART_H
#define STATESTART_H

#include <State.h>

namespace Logic
{
    class StateStart : public State
    {
    public:
        StateStart();
        ~StateStart();
        void reset();
        void update(float deltaTime);
        void render(Graphics::Renderer& renderer);
    };
}

#endif // !STATEGAME_H
