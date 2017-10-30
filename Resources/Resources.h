#pragma once
#include <map>
namespace Resources
{
    namespace Models
    {
        enum Files
        {
            Cube,
            SkySphere,
            Staff,
            Tablet1,
            UnitCube
        };
        const std::map<Files, const char *> Paths =
        {
            {Cube, R"(..\Resources\Models\Cube.hyb)"},
            {SkySphere, R"(..\Resources\Models\SkySphere.hyb)"},
            {Staff, R"(..\Resources\Models\Staff.hyb)"},
            {Tablet1, R"(..\Resources\Models\Tablet1.hyb)"},
            {UnitCube, R"(..\Resources\Models\UnitCube.hyb)"}
        };
    }
    namespace Shaders
    {
        enum Files
        {
            AnimationTest,
            ForwardPlus,
            SimpleForward,
            SkyShader
        };
        const std::map<Files, const char *> Paths =
        {
            {AnimationTest, R"(..\Resources\Shaders\AnimationTest.hlsl)"},
            {ForwardPlus, R"(..\Resources\Shaders\ForwardPlus.hlsl)"},
            {SimpleForward, R"(..\Resources\Shaders\SimpleForward.hlsl)"},
            {SkyShader, R"(..\Resources\Shaders\SkyShader.hlsl)"}
        };
    }
}
