#pragma once
#include <vector>

namespace Hybris
{
    template<typename T>
    class List
    {
    public:
        List();
        List(List & other);
        virtual ~List();

        size_t size;
        T*     data;

        void release();

        T &      operator[](size_t index);
        T const& operator[](size_t index) const;
        List<T>& operator=(List<T> & other);
        List<T>& operator=(std::vector<T> & vector);
    };

    template<typename T>
    List<T>::List()
    {
        size = 0;
        data = nullptr;
    }

    template<typename T>
    List<T>::List(List & other)
    {
        size = other.size;
        data = new T[size];

        for (size_t i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
    }

    template<typename T>
    List<T>::~List()
    {
        release();
    }

    template<typename T>
    T & List<T>::operator[](size_t index)
    {
        return data[index];
    }

    template<typename T>
    T const& List<T>::operator[](size_t index) const
    {
        return data[index];
    }

    template<typename T>
    List<T>& List<T>::operator=(List<T>& other)
    {
        release();
        size = other.size;
        data = new T[size];

        for (size_t i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }

        return *this;
    }

    template<typename T>
    List<T>& List<T>::operator=(std::vector<T>& vector)
    {
        release();

        size = vector.size();
        data = new T[size];

        for (size_t i = 0; i < size; i++)
        {
            data[i] = vector[i];
        }

        return *this;
    }

    template<typename T>
    void List<T>::release()
    {
        if (data != nullptr)
        {
            delete[] data;
            data = nullptr;
            size = 0;
        }
    }
}