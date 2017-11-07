#ifndef STATEBUFFER_H
#define STATEBUFFER_H

#include <Misc\NonCopyable.h>
#include <GameType.h>
#include <string>
#include <queue>

namespace Logic
{
    enum BufferType  { Undefined, Float, String, Int, Bool  };
    
    class _Buffer
    {
    public:
        _Buffer() : type(Undefined), canRemove(false) { }
        ~_Buffer() { }

        bool canRemove;
        StateType sender;
        StateType reciever;
        BufferType type;
        union
        {
            std::string Value_String;
            float       Value_Float;
            int         Value_Int;
            bool        Value_Bool;
        };
    };

    class StateBuffer
    {
    public:
        StateBuffer() { }
        ~StateBuffer() { Clear(); }

        void Clear()
        {
            for (int i = 0; i < m_buffers.size(); i++)
                delete m_buffers[i];
            m_buffers.clear();
        }

        void SendBuffer(void* data, BufferType type, StateType sender, StateType reciever)
        {
            _Buffer* buffer = new _Buffer;
            buffer->type = type;
            buffer->sender = sender;
            buffer->reciever = reciever;
            
            switch (type)
            {
            case BufferType::Float:     buffer->Value_String     = *reinterpret_cast<std::string*>(data);    break;
            case BufferType::String:    buffer->Value_Float      = *reinterpret_cast<float*>(data);          break;
            case BufferType::Int:       buffer->Value_Int        = *reinterpret_cast<int*>(data);            break;
            case BufferType::Bool:      buffer->Value_Bool       = *reinterpret_cast<bool*>(data);           break;
            }
            m_buffers.push_back(buffer);
        }

        void* ReadBuffer(StateType yourType)
        {
            for (size_t i = 0; i < m_buffers.size(); i++)
                if (m_buffers[i]->canRemove)
                    m_buffers.erase(m_buffers.begin() + i);

            if (m_buffers.empty())
                return nullptr;

            for (size_t i = 0; i < m_buffers.size(); i++)
            {
                if (m_buffers[i]->reciever == yourType)
                {
                    printf("State(%d) received a message from State(%d)\n", m_buffers[i]->sender, m_buffers[i]->reciever);
                    m_buffers[i]->canRemove = true;
                    return &m_buffers[i]->Value_Int;
                }
            }

            return nullptr;
        }

        std::vector<_Buffer*> m_buffers;
    };
}

#endif // !STATEBUFFER_H
