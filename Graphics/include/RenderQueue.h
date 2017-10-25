#pragma once
#include <unordered_map>
#include <Resources\Resources.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <memory>
#include <typeindex>
#include <type_traits>

namespace Graphics
{
    class Renderer;
}

struct StaticRenderInfo
{
    static const size_t INSTANCE_CAP = 300;

    Resources::Models::Files model;
    DirectX::SimpleMath::Matrix transform;
};

struct FoliageRenderInfo : StaticRenderInfo
{
};

struct AnimatedRenderInfo : StaticRenderInfo
{
    const char * animationName;
    float animationProgress;
};
//
//template<typename T> struct InstanceCap {};
//#define INSTANCE_CAP(T, V) template<> struct InstanceCap<T> : std::integral_constant<size_t, V> {};
//
//INSTANCE_CAP(StaticRenderInfo, 300)

class RenderQueue_t
{
public:
    template<typename T>
    using InstanceQueue = std::unordered_map<Resources::Models::Files, std::vector<T*>>;

    RenderQueue_t()
    {
        OutputDebugString("\n\nRENDER QUEUE CREATED!\n\n");
    }
    virtual ~RenderQueue_t()
    {
        OutputDebugString("\n\nRENDER QUEUE DESTROEYD!\n\n");
    }

    template<typename T>
    void queue(T * info);

    template<typename T>
    InstanceQueue<T>& getQueue();

    void clearAllQueues();

private:
    struct QueueContainer_t
    {
        size_t instanceCount = 0;
    };

    template<typename T>
    struct QueueContainer : QueueContainer_t
    {
        InstanceQueue<T> instances;
    };

    template<typename T> 
    QueueContainer<T> & getQueueContainer();

    std::unordered_map<std::type_index, std::unique_ptr<QueueContainer_t>> instanceQueues;
};

template<typename T>
void RenderQueue_t::queue(T * info)
{
    QueueContainer<T> & queueContainer = getQueueContainer<T>();

    if (T::INSTANCE_CAP < queueContainer.instanceCount)
        throw std::runtime_error("RenderQueue has exceeded Instance Capacity");

    queueContainer.instances[info->model].push_back(info);
    queueContainer.instanceCount++;
}

template<typename T>
RenderQueue_t::InstanceQueue<T>& RenderQueue_t::getQueue()
{
    return getQueueContainer<T>().instances;
}

template<typename T>
RenderQueue_t::QueueContainer<T>& RenderQueue_t::getQueueContainer()
{
    static_assert(std::is_base_of<StaticRenderInfo, T>::value, "T must have StaticRenderInfo as base type");
    QueueContainer<T> * queueContainer = (QueueContainer<T>*)instanceQueues[typeid(T)].get();

    if (queueContainer == nullptr)
    {
        instanceQueues[typeid(T)] = std::make_unique<QueueContainer<T>>();
        queueContainer = (QueueContainer<T>*)instanceQueues[typeid(T)].get();
    }

    return *queueContainer;
}


extern RenderQueue_t RenderQueue;