#pragma once
#include <cstdint>
#include <fstream>
#include <vector>

namespace Hybris
{
    typedef float time_t;
    typedef int32_t id_t;
    typedef uint32_t count_t;
    typedef char name_t[128];
    typedef float vector3_t[3];
    typedef float vector4_t[4];
    typedef float quaternion_t[4];
    typedef vector4_t matrix4_t[4];

    template<typename T>
    struct List 
    {
        count_t count = 0;
        T* data = nullptr;

        bool operator==(List<T> & other)
        {
            bool same = count == other.count;
            for (count_t i = 0; i < count && same; i++)
            {
                same = same && data[i] == other.data[i];
            }
            return same;
        }

        T& operator[](count_t index) 
        {
            return data[index]; 
        }

        List<T>& operator=(std::vector<T> & vector)
        {
            release();

            count = vector.size();
            data = new T[count];
            memcpy(data, vector.data(), sizeof(T) * count);
            return *this;
        }
        
        void read(std::ifstream & ifile)
        {
            release();

            ifile.read((char*)&count, sizeof(count_t));
            data = new T[count];
            for (count_t i = 0; i < count; i++) 
            {
                data[i].read(ifile); 
            }
        }

        void write(std::ofstream & ofile)
        {
            ofile.write((char*)&count, sizeof(count_t));
            for (count_t i = 0; i < count; i++) 
            {
                data[i].write(ofile); 
            }
        }

        void release()
        {
            if (count != 0)
            {
                for (count_t i = 0; i < count; i++) 
                {
                    data[i].release();
                }

                count = 0;
                delete[] data;
            }
        }
    };

    struct Joint
    {
        id_t id;
        id_t parentId;
        matrix4_t invBindTransform;

        bool operator==(Joint & other)
        {
            bool same = id == other.id;
            same = same && parentId == other.parentId;

            for (count_t x = 0; x < 4 && same; x++)
            { 
                for (count_t y = 0; y < 4 && same; y++)
                { 
                    same = same && invBindTransform[x][y] == other.invBindTransform[x][y]; 
                }
            }

            return same;
        }

        void read(std::ifstream & ifile)
        {
            ifile.read((char*)&id, sizeof(id_t));
            ifile.read((char*)&parentId, sizeof(id_t));
            ifile.read((char*)invBindTransform, sizeof(matrix4_t));
        }

        void write(std::ofstream & ofile)
        {
            ofile.write((char*)&id, sizeof(id_t));
            ofile.write((char*)&parentId, sizeof(id_t));
            ofile.write((char*)invBindTransform, sizeof(matrix4_t));
        }

        void release() {}
    };

    struct JointTransform
    {
        id_t jointId;
        vector3_t translation;
        quaternion_t rotation;
        vector3_t scale;

        bool operator==(JointTransform & other)
        {
            bool same = jointId == other.jointId;

            for (count_t i = 0; i < 3 && same; i++) { same = same && translation[i] == other.translation[i]; }
            for (count_t i = 0; i < 4 && same; i++) { same = same && rotation[i] == other.rotation[i]; }
            for (count_t i = 0; i < 3 && same; i++) { same = same && scale[i] == other.scale[i]; }

            return same;
        }


        void read(std::ifstream & ifile)
        {
            ifile.read((char*)&jointId, sizeof(id_t));
            ifile.read((char*)translation, sizeof(vector3_t));
            ifile.read((char*)rotation, sizeof(quaternion_t));
            ifile.read((char*)scale, sizeof(vector3_t));
        }

        void write(std::ofstream & ofile)
        {
            ofile.write((char*)&jointId, sizeof(id_t));
            ofile.write((char*)translation, sizeof(vector3_t));
            ofile.write((char*)rotation, sizeof(quaternion_t));
            ofile.write((char*)scale, sizeof(vector3_t));
        }
        void release() {}
    };

    struct KeyFrame
    {
        time_t timeStamp;
        List<JointTransform> jointTransforms;

        bool operator==(KeyFrame & other)
        {
            return timeStamp == other.timeStamp && jointTransforms == other.jointTransforms;
        }

        void read(std::ifstream & ifile)
        {
            ifile.read((char*)&timeStamp, sizeof(time_t));
            jointTransforms.read(ifile);
        }

        void write(std::ofstream & ofile)
        {
            ofile.write((char*)&timeStamp, sizeof(time_t));
            jointTransforms.write(ofile);
        }
        void release() 
        {
            jointTransforms.release();
        }
    };

    struct Animation
    {
        name_t name;
        List<KeyFrame> keyFrames;

        bool operator==(Animation & other)
        {
            return strcmp(name, other.name) == 0 && keyFrames == other.keyFrames;
        }

        void read(std::ifstream & ifile)
        {
            ifile.read((char*)name, sizeof(name_t));
            keyFrames.read(ifile);
        }

        void write(std::ofstream & ofile)
        {
            ofile.write((char*)name, sizeof(name_t));
            keyFrames.write(ofile);
        }
        void release() 
        {
            keyFrames.release();
        }
    };

    struct File
    {
        List<Joint> joints;
        List<Animation> animations;

        bool operator==(File & other)
        {
            return joints == other.joints && animations == other.animations;
        }

        void read(std::ifstream & ifile)
        {
            joints.read(ifile);
            animations.read(ifile);
        }

        void write(std::ofstream & ofile)
        {
            joints.write(ofile);
            animations.write(ofile);
        }

        void release() 
        {
            joints.release();
            animations.release();
        }
    };

    static File readFromFile(const char * filePath)
    {
        File file = {};
        std::ifstream ifile(filePath, std::ios::binary);
        file.read(ifile);
        ifile.close();
        return file;
    }

    static void writeToFile(const char * filePath, File & file)
    {
        std::ofstream ofile(filePath, std::ios::binary | std::ios::trunc);
        file.write(ofile);
        ofile.close();
    }

}
