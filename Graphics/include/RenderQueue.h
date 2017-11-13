#pragma once
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <Engine\newd.h>
#include "RenderInfo.h"

class RenderQueue
{
public:
    template<typename T>
    using Queue = std::vector<T>;

    template<typename T>
    using InstancedQueue = std::unordered_map<size_t, Queue<T>>;

    template<typename T>
    inline void queue(T info)
    {
        static_assert(std::is_base_of_v<RenderInfo, T>, "T does not have RenderInfo as base, cant be used with RenderQueue!");

        if constexpr (std::is_base_of_v<StaticRenderInfo, T>)
        {
            InstancedQueueContainer<T> * container = getInstancedQueueContainer<T>();
            container->queue((size_t)info.model, info);
        }
        else
        {
            QueueContainer<T> * container = getQueueContainer<T>();
            container->queue(info);
        }
    }

    template<typename T>
    std::conditional_t<std::is_base_of_v<StaticRenderInfo, T>,InstancedQueue<T>,Queue<T>>&
    getQueue()
    {
        static_assert(std::is_base_of_v<RenderInfo, T>, "T does not have RenderInfo as base, cant be used with RenderQueue!");

        if constexpr (std::is_base_of_v<StaticRenderInfo, T>)
        {
            InstancedQueueContainer<T> * container = getInstancedQueueContainer<T>();
            return container->instances;
        }
        else
        {
            QueueContainer<T> * container = getQueueContainer<T>();
            return container->instances;
        }
    }

    void clearAllQueues()
    {
        for (auto & queue : queues)
        {
            queue.second->clear();
        }
    }

    static RenderQueue& get()
    {
        static RenderQueue instance;
        return instance;
    }
private:
    RenderQueue()
    {
        OutputDebugString("\n\nRENDER QUEUE CREATED!\n\n");
    }
    virtual ~RenderQueue()
    {
        OutputDebugString("\n\nRENDER QUEUE DESTROEYD!\n\n");
        for (auto & instanceQueue : queues)
        {
            delete instanceQueue.second;
        }
    }

    struct QueueContainer_t
    {
        QueueContainer_t() {};
        virtual ~QueueContainer_t() {};
        virtual size_t count() = 0;
        virtual void clear() = 0;
    };

    template<typename T>
    struct QueueContainer : QueueContainer_t
    {
        QueueContainer() {};
        virtual ~QueueContainer() {};
        Queue<T> instances;

        virtual void clear() override
        {
            instances.clear();
        }
        virtual size_t count() override
        {
            return instances.size();
        }
        void queue(T info)
        {
            if (count() > INSTANCE_CAP(T))
                throw std::runtime_error("Instance cap exceeded.");

            instances.push_back(info);
        }
    };

    template<typename T>
    struct InstancedQueueContainer : QueueContainer_t
    {
        InstancedQueueContainer() {};
        virtual ~InstancedQueueContainer() {};
        InstancedQueue<T> instances;
        size_t instanceCount = 0;

        virtual void clear() override
        {
            instanceCount = 0;
            instances.clear();
        }
        virtual size_t count() override
        {
            return instanceCount;
        }
        void queue(size_t key, T info)
        {
            if (count() > INSTANCE_CAP(T))
                throw std::runtime_error("Instance cap exceeded.");

            instances[key].push_back(info);
            instanceCount++;
        }
    };

    template<typename T> 
    QueueContainer<T> * getQueueContainer()
    {
        QueueContainer<T> * queue = nullptr;

        Queues_t::iterator it = queues.find(typeid(T));
        if (it != queues.end())
        {
            queue = (QueueContainer<T>*)it->second;
        }
        else
        {
            queue = newd QueueContainer<T>();
            queues.insert(Queues_t::value_type(typeid(T), queue));
        }

        return queue;
    }

    template<typename T> 
    InstancedQueueContainer<T> * getInstancedQueueContainer()
    {
        InstancedQueueContainer<T> * queue = nullptr;

        Queues_t::iterator it = queues.find(typeid(T));
        if (it != queues.end())
        {
            queue = (InstancedQueueContainer<T>*)it->second;
        }
        else
        {
            queue = newd InstancedQueueContainer<T>();
            queues.insert(Queues_t::value_type(typeid(T), queue));
        }

        return queue;
    }

    typedef std::unordered_map<std::type_index, QueueContainer_t*> Queues_t;
    Queues_t queues;
};
