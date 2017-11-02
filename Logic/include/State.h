#ifndef STATE_H
#define STATE_H

#include <Misc\NonCopyable.h>   
#include <cstdio>               

namespace Graphics { class Renderer; }

namespace Logic
{
    class State : public NonCopyable
    {
    public:
        virtual ~State() = 0;
        virtual void reset() = 0;
        virtual void update(float deltaTime) = 0;
        virtual void render(Graphics::Renderer& renderer) = 0;
    };
}

#endif // !STATE_H
