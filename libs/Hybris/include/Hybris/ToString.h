#pragma once
#include <sstream>
#include "Hybris.h"

namespace Hybris
{
#define PAD "    "
    static struct Pad
    {
        int tabs = 0;
        std::string str()
        {
            std::string str;
            for (int i = 0; i < tabs; i++)
            {
                str += "    ";
            }
            return str;
        }

        void inc()
        {
            tabs++;
        }

        void dec()
        {
            tabs--;
        }        
    } pad;

    template<typename T>
    std::string toString(List<T> & list)
    {
        std::string str;

        str += "\n" + pad.str() + "[";
        pad.inc();
        for (size_t i = 0; i < list.size; i++)
        {
            str += toString(list.data[i]);
            if (i != list.size - 1) { str += ","; }
        }
        pad.dec();
        str += "\n" + pad.str() + "]";
        return str;
    }

    std::string toString(File & file);
    std::string toString(Mesh & mesh);
    std::string toString(Skeleton & skeleton);
    std::string toString(Vertex & vertex);
    std::string toString(Index & index);
    std::string toString(Animation & animation);
    std::string toString(KeyFrame & keyFrame);
    std::string toString(JointTransform & jointTransform);
    std::string toString(Joint & joint);

    //std::string toString(size_t & size);
    std::string toString(time_t & time);
    std::string toString(id_t & id);
    std::string toString(count_t & count);
    std::string toString(name_t & name);
    std::string toString(vector2_t & vector2);
    std::string toString(vector3_t & vector3);
    std::string toString(vector4_t & vector4);
    std::string toString(ivector4_t & ivector4);
    std::string toString(matrix4_t & matrix4);
}