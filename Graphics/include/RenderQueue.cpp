#include "RenderQueue.h"

std::unordered_map<std::type_index, std::unique_ptr<RenderQueue::QueueContainer_t>> RenderQueue::instanceQueues;

void RenderQueue::clearAllQueues()
{
    instanceQueues.clear();
}
