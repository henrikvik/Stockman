#pragma once
#include "IO.h"
#include <vector>

namespace Hybris
{
    template<typename T>
    class List : public IO
    {
    public:
        List();
        List(List & other);
        virtual ~List();

        size_t size();

        T&       operator[](size_t index);
        T& const operator[](size_t index) const;
        List<T>& operator=(List<T> & other);
        List<T>& operator=(std::vector<T> & vector);
        bool     operator==(List<T> & other);

        // Inherited via IO
        virtual void read(std::ifstream & file) override;
        virtual void write(std::ofstream & file) override;
    private:
        size_t count;
        T* data;

        void release();

    };

    template<typename T>
    List<T>::List()
    {
        count = 0;
        data = nullptr;
    }

    template<typename T>
    List<T>::List(List & other)
    {
        count = other.count;
        data = new T[count];

        for (size_t i = 0; i < count; i++)
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
    size_t List<T>::size()
    {
        return count;
    }

    template<typename T>
    T & List<T>::operator[](size_t index)
    {
        return data[index];
    }

    template<typename T>
    T & const List<T>::operator[](size_t index) const
    { 
        return data[index]; 
    }

    template<typename T>
    List<T>& List<T>::operator=(List<T>& other)
    {
        release();
        count = other.count;

        data = new T[count];
        for (size_t i = 0; i < count; i++)
        {
            data[i] = other.data[i];
        }

        return *this;
    }

    template<typename T>
    List<T>& List<T>::operator=(std::vector<T>& vector)
    {
        release();

        count = vector.size();
        data = new T[count];

        for (size_t i = 0; i < count; i++)
        {
            data[i] = vector[i];
        }

        return *this;
    }

    template<typename T>
    bool List<T>::operator==(List<T>& other)
    {
        bool same = count == other.count;
        for (count_t i = 0; i < count && same; i++)
        {
            same = same && data[i] == other.data[i];
        }
        return same;
    }

    template<typename T>
    void List<T>::release()
    {
        if (data)
        {
            delete[] data;
            data = nullptr;
            count = 0;
        }
    }

    // Inherited via IO
    template<typename T>
    void List<T>::read(std::ifstream & file)
    {
        release();
        file.read((char*)&count, sizeof(count));

        for (size_t i = 0; i < count; i++)
        {
            data[i].read(file);
        }

         //file.read((char*)data, sizeof(T) * count);

    }

    template<typename T>
    void List<T>::write(std::ofstream & file)
    {
        file.write((char*)&count, sizeof(count));

        for (size_t i = 0; i < count; i++)
        {
            data[i].write(file);
        }
        //file.write((char*)data, sizeof(T) * count);
    }
}