#include "RenderQueue.h"

void RenderQueue_t::clearAllQueues()
{
    for (auto & queue : instanceQueues)
    {
        queue.second->clear();
    }
    //instanceQueues.clear();
}