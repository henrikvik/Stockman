#pragma once
#include <fstream>
#include "Hybris.h"

namespace Hybris
{
    template<typename T>
    void write(std::ofstream & ofile, List<T> & list)
    {
        write(ofile, list.size);
        for (size_t i = 0; i < list.size; i++)
        {
            write(ofile, list.data[i]);
        }
    }
    void write(std::ofstream & ofile, File & file);
    void write(std::ofstream & ofile, Material & mesh);
    void write(std::ofstream & ofile, Texture & texture);
    void write(std::ofstream & ofile, Mesh & mesh);
    void write(std::ofstream & ofile, Skeleton & skeleton);
    void write(std::ofstream & ofile, Vertex & vertex);
    void write(std::ofstream & ofile, Index & index);
    void write(std::ofstream & ofile, Animation & animation);
    void write(std::ofstream & ofile, KeyFrame & keyFrame);
    void write(std::ofstream & ofile, JointTransform & jointTransform);
    void write(std::ofstream & ofile, Joint & joint);
    void write(std::ofstream & ofile, Hitbox & data);
    void write(std::ofstream & ofile, FileWithHitbox & data);

    void write(std::ofstream & ofile, size_t & size);
    void write(std::ofstream & ofile, time_t & time);
    void write(std::ofstream & ofile, id_t & id);
    void write(std::ofstream & ofile, count_t & count);
    void write(std::ofstream & ofile, name_t & name);
    void write(std::ofstream & ofile, vector2_t & vector2);
    void write(std::ofstream & ofile, vector3_t & vector3);
    void write(std::ofstream & ofile, vector4_t & vector4);
    //void write(std::ofstream & ofile, quaternion_t & quaternion);
    void write(std::ofstream & ofile, matrix4_t & matrix4);
    void write(std::ofstream & ofile, ivector4_t & ivector4);
    void write(std::ofstream & ofile, byte_t & byte);

}
