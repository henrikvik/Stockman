#ifndef CREDITS_H
#define CREDITS_H

#include <vector>
#include <Graphics\include\RenderQueue.h>

namespace Logic {
    class Credits {
    private:
        std::vector<TextRenderInfo> info;
        std::vector<std::wstring> infoText;
    public:
        Credits();

        void update(float deltaTime);
        void render() const;
    };
}

#endif