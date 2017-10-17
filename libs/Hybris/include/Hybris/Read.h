#pragma once
#include <fstream>
#include "Hybris.h"

namespace Hybris
{
    template<typename T>
    void read(std::ifstream & ifile, List<T> & list)
    {
        list.release();
        read(ifile, list.size);
        list.data = new T[list.size];

        for (size_t i = 0; i < list.size; i++)
        {
            read(ifile, list.data[i]);
        }
    }
    void read(std::ifstream & ifile, File & file);
    void read(std::ifstream & ifile, Mesh & mesh);
    void read(std::ifstream & ifile, Skeleton & skeleton);
    void read(std::ifstream & ifile, Vertex & vertex);
    void read(std::ifstream & ifile, Index & index);
    void read(std::ifstream & ifile, Animation & animation);
    void read(std::ifstream & ifile, KeyFrame & keyFrame);
    void read(std::ifstream & ifile, JointTransform & jointTransform);
    void read(std::ifstream & ifile, Joint & joint);

    void read(std::ifstream & ifile, size_t & size);
    void read(std::ifstream & ifile, time_t & time);
    void read(std::ifstream & ifile, id_t & id);
    void read(std::ifstream & ifile, count_t & count);
    void read(std::ifstream & ifile, name_t & name);
    void read(std::ifstream & ifile, vector2_t & vector2);
    void read(std::ifstream & ifile, vector3_t & vector3);
    void read(std::ifstream & ifile, vector4_t & vector4);
    //void read(std::ifstream & ifile, quaternion_t & quaternion);
    void read(std::ifstream & ifile, matrix4_t & matrix4);
    void read(std::ifstream & ifile, ivector4_t & ivector4);
}
