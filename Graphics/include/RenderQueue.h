#pragma once
#include <unordered_map>
#include <Resources\Resources.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <memory>
#include <typeindex>
#include <type_traits>
#include <Engine\newd.h>

namespace Graphics
{
    class Renderer;
}

struct RenderInfo
{
    static constexpr size_t INSTANCE_CAP = 3000;
};

struct StaticRenderInfo : RenderInfo
{
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


class RenderQueue_t
{
public:

    template<typename T>
    using InstanceQueue = std::unordered_map<Resources::Models::Files, std::vector<T*>>;

    template<typename T>
    void queue(T * info);

    template<typename T>
    InstanceQueue<T>& getQueue();

    void clearAllQueues();

    static RenderQueue_t& get()
    {
        static RenderQueue_t instance;
        return instance;
    }
private:
    RenderQueue_t()
    {
        OutputDebugString("\n\nRENDER QUEUE CREATED!\n\n");
    }
    virtual ~RenderQueue_t()
    {
        OutputDebugString("\n\nRENDER QUEUE DESTROEYD!\n\n");
        for (auto & instanceQueue : instanceQueues)
        {
            delete instanceQueue.second;
        }
    }


    struct QueueContainer_t
    {
        virtual ~QueueContainer_t() {};

        size_t instanceCount = 0;
        virtual void clear() = 0;
    };

    template<typename T>
    struct QueueContainer : QueueContainer_t
    {
        virtual ~QueueContainer() {};
        InstanceQueue<T> instances;
        virtual void clear() override
        {
            instanceCount = 0;
            instances.clear();
        }
    };

    template<typename T> 
    QueueContainer<T> * getQueueContainer();

    typedef std::unordered_map<std::type_index, QueueContainer_t *> InstanceQueues_t;
    InstanceQueues_t instanceQueues;
};

template<typename T>
void RenderQueue_t::queue(T * info)
{
    QueueContainer<T> * queueContainer = getQueueContainer<T>();

    if (T::INSTANCE_CAP < queueContainer->instanceCount)
        throw std::runtime_error("RenderQueue has exceeded Instance Capacity");

    queueContainer->instances[info->model].push_back(info);
    queueContainer->instanceCount++;
}

template<typename T>
RenderQueue_t::InstanceQueue<T>& RenderQueue_t::getQueue()
{
    return getQueueContainer<T>()->instances;
}

template<typename T>
RenderQueue_t::QueueContainer<T> * RenderQueue_t::getQueueContainer()
{
    static_assert(std::is_base_of<RenderInfo, T>::value, "T must have StaticRenderInfo as base type");

    InstanceQueues_t::iterator it = instanceQueues.find(typeid(T));
    if (it == instanceQueues.end())
    {
        it = instanceQueues.insert(
            InstanceQueues_t::value_type(typeid(T), newd QueueContainer<T>())
        ).first;
    }

    return dynamic_cast<QueueContainer<T>*>(it->second);
}