#pragma once
#include <d3d11.h>
#include "AnimatedVertex.h"
#include "Skeleton.h"
#include "Animation.h"

struct  AnimatedTestCube
{
    AnimatedTestCube(ID3D11Device * device)
    {
        AnimatedVertex vertices[] =
        {
            // FRONT
            { { -1, -1, -1 * -1 },{  0,  0, -1 * -1 },{ 1, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1,  1, -1 * -1 },{  0,  0, -1 * -1 },{ 1, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1, -1, -1 * -1 },{  0,  0, -1 * -1 },{ 1, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1, -1, -1 * -1 },{  0,  0, -1 * -1 },{ 1, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1,  1, -1 * -1 },{  0,  0, -1 * -1 },{ 1, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1,  1, -1 * -1 },{  0,  0, -1 * -1 },{ 1, 0, 0, 0 },{ 1, 0, 0, 0 } },
            // TOP
            { { -1,  1, -1 * -1 },{  0,  1,  0 * -1 },{ 2, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1,  1,  1 * -1 },{  0,  1,  0 * -1 },{ 2, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1,  1, -1 * -1 },{  0,  1,  0 * -1 },{ 2, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1,  1, -1 * -1 },{  0,  1,  0 * -1 },{ 2, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1,  1,  1 * -1 },{  0,  1,  0 * -1 },{ 2, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1,  1,  1 * -1 },{  0,  1,  0 * -1 },{ 2, 0, 0, 0 },{ 1, 0, 0, 0 } },
            // LEFT
            { { -1, -1,  1 * -1 },{ -1,  0,  0 * -1 },{ 3, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1,  1,  1 * -1 },{ -1,  0,  0 * -1 },{ 3, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1, -1, -1 * -1 },{ -1,  0,  0 * -1 },{ 3, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1, -1, -1 * -1 },{ -1,  0,  0 * -1 },{ 3, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1,  1,  1 * -1 },{ -1,  0,  0 * -1 },{ 3, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1,  1, -1 * -1 },{ -1,  0,  0 * -1 },{ 3, 0, 0, 0 },{ 1, 0, 0, 0 } },
            // RIGHT
            { {  1,  1,  1 * -1 },{  1,  0,  0 * -1 },{ 4, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1, -1,  1 * -1 },{  1,  0,  0 * -1 },{ 4, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1, -1, -1 * -1 },{  1,  0,  0 * -1 },{ 4, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1,  1,  1 * -1 },{  1,  0,  0 * -1 },{ 4, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1, -1, -1 * -1 },{  1,  0,  0 * -1 },{ 4, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1,  1, -1 * -1 },{  1,  0,  0 * -1 },{ 4, 0, 0, 0 },{ 1, 0, 0, 0 } },
            // BACK
            { { -1,  1,  1 * -1 },{  0,  0,  1 * -1 },{ 5, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1, -1,  1 * -1 },{  0,  0,  1 * -1 },{ 5, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1, -1,  1 * -1 },{  0,  0,  1 * -1 },{ 5, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1,  1,  1 * -1 },{  0,  0,  1 * -1 },{ 5, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1, -1,  1 * -1 },{  0,  0,  1 * -1 },{ 5, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1,  1,  1 * -1 },{  0,  0,  1 * -1 },{ 5, 0, 0, 0 },{ 1, 0, 0, 0 } },
            // BOTTOM
            { { -1, -1,  1 * -1 },{  0, -1,  0 * -1 },{ 6, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1, -1, -1 * -1 },{  0, -1,  0 * -1 },{ 6, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1, -1, -1 * -1 },{  0, -1,  0 * -1 },{ 6, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { { -1, -1,  1 * -1 },{  0, -1,  0 * -1 },{ 6, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1, -1, -1 * -1 },{  0, -1,  0 * -1 },{ 6, 0, 0, 0 },{ 1, 0, 0, 0 } },
            { {  1, -1,  1 * -1 },{  0, -1,  0 * -1 },{ 6, 0, 0, 0 },{ 1, 0, 0, 0 } }
        };

        D3D11_SUBRESOURCE_DATA subData = {};
        subData.pSysMem = vertices;

        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(vertices);
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        device->CreateBuffer(&desc, &subData, &vertexBuffer);

        offset = 0;
        stride = sizeof(AnimatedVertex);
        vertexCount = 36;
    }

    ~AnimatedTestCube()
    {
        vertexBuffer->Release();
    }
    UINT offset;
    UINT stride;
    UINT vertexCount;

    ID3D11Buffer * vertexBuffer;
    inline operator ID3D11Buffer*() { return vertexBuffer; }
    inline operator ID3D11Buffer**() { return &vertexBuffer; }
};

static Skeleton testSkeleton = []() {
    Joint rootJoint = { 0, Matrix() };
    rootJoint.addChild({ 1, Matrix::CreateTranslation( 0, 0, -1) });
    rootJoint.addChild({ 2, Matrix::CreateTranslation( 0, -1, 0) });
    rootJoint.addChild({ 3, Matrix::CreateTranslation(1, 0, 0) });
    rootJoint.addChild({ 4, Matrix::CreateTranslation( -1, 0, 0) });
    rootJoint.addChild({ 5, Matrix::CreateTranslation( 0, 0,1) });
    rootJoint.addChild({ 6, Matrix::CreateTranslation( 0,1, 0) });

    return Skeleton(rootJoint, 7);
}();

static Animation testAnimation = [](){
    std::vector<KeyFrame> keyFrames;

   
    KeyFrame closed(0.0f, {
        { Vector3(0, 0, 0), Quaternion() },

        { Vector3(0, 0, 1), Quaternion() },
        { Vector3(0, 1, 0), Quaternion() },
        { Vector3(-1, 0, 0), Quaternion() },
        { Vector3(1, 0, 0), Quaternion() },
        { Vector3(0, 0, -1), Quaternion() },
        { Vector3(0, -1, 0), Quaternion() }
    });


    KeyFrame open(1.0f, {
        { Vector3( 0, 0, 0), Quaternion() },

        { Vector3( 0, 0, 2), Quaternion() },
        { Vector3( 0, 2, 0), Quaternion() },
        { Vector3(-2, 0, 0), Quaternion() },
        { Vector3( 2, 0, 0), Quaternion() },
        { Vector3( 0, 0,-2), Quaternion() },
        { Vector3( 0,-2, 0), Quaternion() }
    });

    KeyFrame closed2(2.0f, {
        { Vector3(0, 0, 0), Quaternion() },

        { Vector3(0, 0, 1), Quaternion() },
        { Vector3(0, 1, 0), Quaternion() },
        { Vector3(-1, 0, 0), Quaternion() },
        { Vector3(1, 0, 0), Quaternion() },
        { Vector3(0, 0, -1), Quaternion() },
        { Vector3(0, -1, 0), Quaternion() }
    });

    return Animation(3.0f, { closed, open, closed2 });
}();