#pragma once
#include <map>
namespace Resources
{
    namespace Fonts
    {
        enum Files
        {
            KG14,
            KG18,
            KG26
        };
        const std::map<Files, const char *> Paths =
        {
            {KG14, R"(..\Resources\Fonts\KG14.spritefont)"},
            {KG18, R"(..\Resources\Fonts\KG18.spritefont)"},
            {KG26, R"(..\Resources\Fonts\KG26.spritefont)"}
        };
        
        Files toEnum(const char * str)
        {
            if (strcmp(str, "KG14") == 0) return KG14;
            if (strcmp(str, "KG18") == 0) return KG18;
            if (strcmp(str, "KG26") == 0) return KG26;
            throw "Could not find matching enum.";
        }
    }
    namespace Maps
    {
        enum Files
        {
            Scene
        };
        const std::map<Files, const char *> Paths =
        {
            {Scene, R"(..\Resources\Maps\Scene.toml)"}
        };
        
        Files toEnum(const char * str)
        {
            if (strcmp(str, "Scene") == 0) return Scene;
            throw "Could not find matching enum.";
        }
    }
    namespace Models
    {
        enum Files
        {
            Crossbow,
            Hammer,
            SkySphere,
            Staff,
            StaticSummon,
            Tablet1,
            UnitCube
        };
        const std::map<Files, const char *> Paths =
        {
            {Crossbow, R"(..\Resources\Models\Crossbow.hyb)"},
            {Hammer, R"(..\Resources\Models\Hammer.hyb)"},
            {SkySphere, R"(..\Resources\Models\SkySphere.hyb)"},
            {Staff, R"(..\Resources\Models\Staff.hyb)"},
            {StaticSummon, R"(..\Resources\Models\StaticSummon.hyb)"},
            {Tablet1, R"(..\Resources\Models\Tablet1.hyb)"},
            {UnitCube, R"(..\Resources\Models\UnitCube.hyb)"}
        };
        
        Files toEnum(const char * str)
        {
            if (strcmp(str, "Crossbow") == 0) return Crossbow;
            if (strcmp(str, "Hammer") == 0) return Hammer;
            if (strcmp(str, "SkySphere") == 0) return SkySphere;
            if (strcmp(str, "Staff") == 0) return Staff;
            if (strcmp(str, "StaticSummon") == 0) return StaticSummon;
            if (strcmp(str, "Tablet1") == 0) return Tablet1;
            if (strcmp(str, "UnitCube") == 0) return UnitCube;
            throw "Could not find matching enum.";
        }
    }
    namespace Shaders
    {
        enum Files
        {
            BulletTime,
            DebugShader,
            DoFBlur1,
            DoFBlur2,
            DoFCreateCoC,
            DoFGlue,
            ForwardPlus,
            ForwardPlus_PS,
            ForwardPlus_VS_Static,
            FullscreenQuad,
            GlowBlurHorizontal,
            GlowBlurVertical,
            GlowDownSampler,
            GlowMerger,
            GlowMipCombinder,
            LightGridCulling,
            SkyShader,
            SnowShader,
            SpriteShader,
            SSAOComputeShader,
            SSAOGaussianBlurHorizontal,
            SSAOGaussianBlurVertical,
            SSAOMerger
        };
        const std::map<Files, const char *> Paths =
        {
            {BulletTime, R"(..\Resources\Shaders\BulletTime.hlsl)"},
            {DebugShader, R"(..\Resources\Shaders\DebugShader.hlsl)"},
            {DoFBlur1, R"(..\Resources\Shaders\DoFBlur1.hlsl)"},
            {DoFBlur2, R"(..\Resources\Shaders\DoFBlur2.hlsl)"},
            {DoFCreateCoC, R"(..\Resources\Shaders\DoFCreateCoC.hlsl)"},
            {DoFGlue, R"(..\Resources\Shaders\DoFGlue.hlsl)"},
            {ForwardPlus, R"(..\Resources\Shaders\ForwardPlus.hlsl)"},
            {ForwardPlus_PS, R"(..\Resources\Shaders\ForwardPlus_PS.hlsl)"},
            {ForwardPlus_VS_Static, R"(..\Resources\Shaders\ForwardPlus_VS_Static.hlsl)"},
            {FullscreenQuad, R"(..\Resources\Shaders\FullscreenQuad.hlsl)"},
            {GlowBlurHorizontal, R"(..\Resources\Shaders\GlowBlurHorizontal.hlsl)"},
            {GlowBlurVertical, R"(..\Resources\Shaders\GlowBlurVertical.hlsl)"},
            {GlowDownSampler, R"(..\Resources\Shaders\GlowDownSampler.hlsl)"},
            {GlowMerger, R"(..\Resources\Shaders\GlowMerger.hlsl)"},
            {GlowMipCombinder, R"(..\Resources\Shaders\GlowMipCombinder.hlsl)"},
            {LightGridCulling, R"(..\Resources\Shaders\LightGridCulling.hlsl)"},
            {SkyShader, R"(..\Resources\Shaders\SkyShader.hlsl)"},
            {SnowShader, R"(..\Resources\Shaders\SnowShader.hlsl)"},
            {SpriteShader, R"(..\Resources\Shaders\SpriteShader.hlsl)"},
            {SSAOComputeShader, R"(..\Resources\Shaders\SSAOComputeShader.hlsl)"},
            {SSAOGaussianBlurHorizontal, R"(..\Resources\Shaders\SSAOGaussianBlurHorizontal.hlsl)"},
            {SSAOGaussianBlurVertical, R"(..\Resources\Shaders\SSAOGaussianBlurVertical.hlsl)"},
            {SSAOMerger, R"(..\Resources\Shaders\SSAOMerger.hlsl)"}
        };
        
        Files toEnum(const char * str)
        {
            if (strcmp(str, "BulletTime") == 0) return BulletTime;
            if (strcmp(str, "DebugShader") == 0) return DebugShader;
            if (strcmp(str, "DoFBlur1") == 0) return DoFBlur1;
            if (strcmp(str, "DoFBlur2") == 0) return DoFBlur2;
            if (strcmp(str, "DoFCreateCoC") == 0) return DoFCreateCoC;
            if (strcmp(str, "DoFGlue") == 0) return DoFGlue;
            if (strcmp(str, "ForwardPlus") == 0) return ForwardPlus;
            if (strcmp(str, "ForwardPlus_PS") == 0) return ForwardPlus_PS;
            if (strcmp(str, "ForwardPlus_VS_Static") == 0) return ForwardPlus_VS_Static;
            if (strcmp(str, "FullscreenQuad") == 0) return FullscreenQuad;
            if (strcmp(str, "GlowBlurHorizontal") == 0) return GlowBlurHorizontal;
            if (strcmp(str, "GlowBlurVertical") == 0) return GlowBlurVertical;
            if (strcmp(str, "GlowDownSampler") == 0) return GlowDownSampler;
            if (strcmp(str, "GlowMerger") == 0) return GlowMerger;
            if (strcmp(str, "GlowMipCombinder") == 0) return GlowMipCombinder;
            if (strcmp(str, "LightGridCulling") == 0) return LightGridCulling;
            if (strcmp(str, "SkyShader") == 0) return SkyShader;
            if (strcmp(str, "SnowShader") == 0) return SnowShader;
            if (strcmp(str, "SpriteShader") == 0) return SpriteShader;
            if (strcmp(str, "SSAOComputeShader") == 0) return SSAOComputeShader;
            if (strcmp(str, "SSAOGaussianBlurHorizontal") == 0) return SSAOGaussianBlurHorizontal;
            if (strcmp(str, "SSAOGaussianBlurVertical") == 0) return SSAOGaussianBlurVertical;
            if (strcmp(str, "SSAOMerger") == 0) return SSAOMerger;
            throw "Could not find matching enum.";
        }
    }
    namespace Textures
    {
        enum Files
        {
            Backbutton,
            Color_pick_icons,
            Crosshair,
            gameOverMenuButtons,
            gameOverTexture,
            Gamesheet,
            Grid,
            HPBars,
            HUDIcons,
            mainMenuButton,
            mainmenupicture,
            MissingTexture,
            randomNormals,
            SettingsMenuButtons,
            Skillpickbackground,
            Skillpicksheet,
            Spellsheet,
            weaponsheet
        };
        const std::map<Files, const char *> Paths =
        {
            {Backbutton, R"(..\Resources\Textures\Backbutton.dds)"},
            {Color_pick_icons, R"(..\Resources\Textures\Color_pick_icons.dds)"},
            {Crosshair, R"(..\Resources\Textures\Crosshair.dds)"},
            {gameOverMenuButtons, R"(..\Resources\Textures\gameOverMenuButtons.png)"},
            {gameOverTexture, R"(..\Resources\Textures\gameOverTexture.png)"},
            {Gamesheet, R"(..\Resources\Textures\Gamesheet.dds)"},
            {Grid, R"(..\Resources\Textures\Grid.png)"},
            {HPBars, R"(..\Resources\Textures\HPBars.dds)"},
            {HUDIcons, R"(..\Resources\Textures\HUDIcons.dds)"},
            {mainMenuButton, R"(..\Resources\Textures\mainMenuButton.png)"},
            {mainmenupicture, R"(..\Resources\Textures\mainmenupicture.dds)"},
            {MissingTexture, R"(..\Resources\Textures\MissingTexture.png)"},
            {randomNormals, R"(..\Resources\Textures\randomNormals.png)"},
            {SettingsMenuButtons, R"(..\Resources\Textures\SettingsMenuButtons.png)"},
            {Skillpickbackground, R"(..\Resources\Textures\Skillpickbackground.dds)"},
            {Skillpicksheet, R"(..\Resources\Textures\Skillpicksheet.dds)"},
            {Spellsheet, R"(..\Resources\Textures\Spellsheet.dds)"},
            {weaponsheet, R"(..\Resources\Textures\weaponsheet.dds)"}
        };
        
        Files toEnum(const char * str)
        {
            if (strcmp(str, "Backbutton") == 0) return Backbutton;
            if (strcmp(str, "Color_pick_icons") == 0) return Color_pick_icons;
            if (strcmp(str, "Crosshair") == 0) return Crosshair;
            if (strcmp(str, "gameOverMenuButtons") == 0) return gameOverMenuButtons;
            if (strcmp(str, "gameOverTexture") == 0) return gameOverTexture;
            if (strcmp(str, "Gamesheet") == 0) return Gamesheet;
            if (strcmp(str, "Grid") == 0) return Grid;
            if (strcmp(str, "HPBars") == 0) return HPBars;
            if (strcmp(str, "HUDIcons") == 0) return HUDIcons;
            if (strcmp(str, "mainMenuButton") == 0) return mainMenuButton;
            if (strcmp(str, "mainmenupicture") == 0) return mainmenupicture;
            if (strcmp(str, "MissingTexture") == 0) return MissingTexture;
            if (strcmp(str, "randomNormals") == 0) return randomNormals;
            if (strcmp(str, "SettingsMenuButtons") == 0) return SettingsMenuButtons;
            if (strcmp(str, "Skillpickbackground") == 0) return Skillpickbackground;
            if (strcmp(str, "Skillpicksheet") == 0) return Skillpicksheet;
            if (strcmp(str, "Spellsheet") == 0) return Spellsheet;
            if (strcmp(str, "weaponsheet") == 0) return weaponsheet;
            throw "Could not find matching enum.";
        }
    }
}
