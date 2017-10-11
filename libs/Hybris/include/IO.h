#pragma once
#include <fstream>

namespace Hybris
{
    class IO
    {
    public:
        virtual void read(std::ifstream & file) = 0;
        virtual void write(std::ofstream & file) = 0;
    };
}