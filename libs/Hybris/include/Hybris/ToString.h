#pragma once
#include <sstream>
#include "Hybris.h"

namespace Hybris
{
#define PAD "|  "
#define DPAD "\b\b\b"

    template<typename T>
    std::string toString(List<T> & list, std::string pad)
    {
        std::string str;

        for (size_t i = 0; i < list.size; i++)
        {
            str += "\n" + pad;
            str += "\n" + pad + "[" + std::to_string(i) + "]" + "\n";
            str+=toString(list.data[i], pad);
        }
        return str;
    }

    std::string toString(File & file);
    std::string toString(Mesh & mesh, std::string pad);
    std::string toString(Skeleton & skeleton, std::string pad);
    std::string toString(Vertex & vertex, std::string pad);
    std::string toString(Index & index, std::string pad);
    std::string toString(Animation & animation, std::string pad);
    std::string toString(KeyFrame & keyFrame, std::string pad);
    std::string toString(JointTransform & jointTransform, std::string pad);
    std::string toString(Joint & joint, std::string pad);

    std::string toString(size_t & size);
    std::string toString(time_t & time);
    std::string toString(id_t & id);
    std::string toString(count_t & count);
    std::string toString(name_t & name);
    std::string toString(vector2_t & vector2);
    std::string toString(vector3_t & vector3);
    std::string toString(vector4_t & vector4);
    std::string toString(ivector4_t & ivector4);
    std::string toString(matrix4_t & matrix4, std::string pad);
}