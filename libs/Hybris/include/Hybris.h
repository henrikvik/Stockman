#pragma once
#include "Hybris\Typedefs.h"
#include "Hybris\List.h"
#include "Hybris\IO.h"

namespace Hybris
{
    struct Joint : IO
    {
        id_t id;
        id_t parentId;
        matrix4_t invBindTransform;

        // Inherited via IO
        virtual void read(std::ifstream & file) override;
        virtual void write(std::ofstream & file) override;
    };

    struct JointTransform : IO
    {
        id_t jointId;
        vector3_t translation;
        quaternion_t rotation;
        vector3_t scale;

        // Inherited via IO
        virtual void read(std::ifstream & file) override;
        virtual void write(std::ofstream & file) override;
    };

    struct KeyFrame : IO
    {
        time_t timeStamp;
        List<JointTransform> jointTransforms;

        // Inherited via IO
        virtual void read(std::ifstream & file) override;
        virtual void write(std::ofstream & file) override;
    };

    struct Animation : IO
    {
        name_t name;
        List<KeyFrame> keyFrames;

        // Inherited via IO
        virtual void read(std::ifstream & file) override;
        virtual void write(std::ofstream & file) override;
    };

    struct Index : IO
    {
        id_t vertexId;

        // Inherited via IO
        virtual void read(std::ifstream & file) override;
        virtual void write(std::ofstream & file) override;
    };

    struct Vertex : IO
    {
        vector3_t  position;
        vector3_t  normal;
        vector3_t  binormal;
        vector3_t  tangent;
        vector2_t  uv;
        ivector4_t jointIds;
        vector4_t  jointWeights;

        // Inherited via IO
        virtual void read(std::ifstream & file) override;
        virtual void write(std::ofstream & file) override;
    };

    struct Mesh : IO
    {
        List<Vertex> vertices;
        List<Index> indices;

        // Inherited via IO
        virtual void read(std::ifstream & file) override;
        virtual void write(std::ofstream & file) override;
    };

    struct File : IO
    {
        Mesh mesh;
        List<Joint> joints;
        List<Animation> animations;

        // Inherited via IO
        virtual void read(std::ifstream & file) override;
        virtual void write(std::ofstream & file) override;
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
