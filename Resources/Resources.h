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
    }
    namespace Maps
    {
        enum Files
        {
            IslandScene,
            Scene
        };
        const std::map<Files, const char *> Paths =
        {
            {IslandScene, R"(..\Resources\Maps\IslandScene.toml)"},
            {Scene, R"(..\Resources\Maps\Scene.toml)"}
        };
    }
    namespace Models
    {
        enum Files
        {
            Ammocrystal,
            AmmoPackCrossBolt,
            AnimatedSummonUnit,
            Barrel,
            Crossbow,
            Crossbowbolt,
            Crossbowbolts,
            Firepit,
            Hammer,
            Island,
            MenuScene,
            Necromancer,
            SkySphere,
            Staff,
            StaticSummon,
            SummonUnitWithAnim,
            Tablet1,
            UnitCube
        };
        const std::map<Files, const char *> Paths =
        {
            {Ammocrystal, R"(..\Resources\Models\Ammocrystal.hyb)"},
            {AmmoPackCrossBolt, R"(..\Resources\Models\AmmoPackCrossBolt.hyb)"},
            {AnimatedSummonUnit, R"(..\Resources\Models\AnimatedSummonUnit.hyb)"},
            {Barrel, R"(..\Resources\Models\Barrel.hyb)"},
            {Crossbow, R"(..\Resources\Models\Crossbow.hyb)"},
            {Crossbowbolt, R"(..\Resources\Models\Crossbowbolt.hyb)"},
            {Crossbowbolts, R"(..\Resources\Models\Crossbowbolts.hyb)"},
            {Firepit, R"(..\Resources\Models\Firepit.hyb)"},
            {Hammer, R"(..\Resources\Models\Hammer.hyb)"},
            {Island, R"(..\Resources\Models\Island.hyb)"},
            {MenuScene, R"(..\Resources\Models\MenuScene.hyb)"},
            {Necromancer, R"(..\Resources\Models\Necromancer.hyb)"},
            {SkySphere, R"(..\Resources\Models\SkySphere.hyb)"},
            {Staff, R"(..\Resources\Models\Staff.hyb)"},
            {StaticSummon, R"(..\Resources\Models\StaticSummon.hyb)"},
            {SummonUnitWithAnim, R"(..\Resources\Models\SummonUnitWithAnim.hyb)"},
            {Tablet1, R"(..\Resources\Models\Tablet1.hyb)"},
            {UnitCube, R"(..\Resources\Models\UnitCube.hyb)"}
        };
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
            ForwardPlus_VS_Animated,
            ForwardPlus_VS_Foliage,
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
            {ForwardPlus_VS_Animated, R"(..\Resources\Shaders\ForwardPlus_VS_Animated.hlsl)"},
            {ForwardPlus_VS_Foliage, R"(..\Resources\Shaders\ForwardPlus_VS_Foliage.hlsl)"},
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
    }
    namespace Textures
    {
        enum Files
        {
            Backbutton,
            CardBackground,
            Crosshair,
            Gameover,
            gameOverMenuButtons,
            gameOverTexture,
            Gamesheet,
            Grid,
            Highscore,
            Highscoretext,
            HPBars,
            HUDIcons,
            iconsheet,
            IntroScreen,
            Loadingscreen,
            MainmenuClean,
            mainmenupicture,
            Mainmenutext,
            MissingTexture,
            Pausetext,
            randomNormals,
            Settings,
            SettingsMenuButtons,
            Skillpickbackground,
            Skillpicksheet,
            Spellsheet,
            WaveComplete,
            weaponsheet
        };
        const std::map<Files, const char *> Paths =
        {
            {Backbutton, R"(..\Resources\Textures\Backbutton.dds)"},
            {CardBackground, R"(..\Resources\Textures\CardBackground.dds)"},
            {Crosshair, R"(..\Resources\Textures\Crosshair.dds)"},
            {Gameover, R"(..\Resources\Textures\Gameover.dds)"},
            {gameOverMenuButtons, R"(..\Resources\Textures\gameOverMenuButtons.png)"},
            {gameOverTexture, R"(..\Resources\Textures\gameOverTexture.png)"},
            {Gamesheet, R"(..\Resources\Textures\Gamesheet.dds)"},
            {Grid, R"(..\Resources\Textures\Grid.png)"},
            {Highscore, R"(..\Resources\Textures\Highscore.dds)"},
            {Highscoretext, R"(..\Resources\Textures\Highscoretext.dds)"},
            {HPBars, R"(..\Resources\Textures\HPBars.dds)"},
            {HUDIcons, R"(..\Resources\Textures\HUDIcons.dds)"},
            {iconsheet, R"(..\Resources\Textures\iconsheet.dds)"},
            {IntroScreen, R"(..\Resources\Textures\IntroScreen.dds)"},
            {Loadingscreen, R"(..\Resources\Textures\Loadingscreen.dds)"},
            {MainmenuClean, R"(..\Resources\Textures\MainmenuClean.dds)"},
            {mainmenupicture, R"(..\Resources\Textures\mainmenupicture.dds)"},
            {Mainmenutext, R"(..\Resources\Textures\Mainmenutext.dds)"},
            {MissingTexture, R"(..\Resources\Textures\MissingTexture.png)"},
            {Pausetext, R"(..\Resources\Textures\Pausetext.dds)"},
            {randomNormals, R"(..\Resources\Textures\randomNormals.png)"},
            {Settings, R"(..\Resources\Textures\Settings.dds)"},
            {SettingsMenuButtons, R"(..\Resources\Textures\SettingsMenuButtons.png)"},
            {Skillpickbackground, R"(..\Resources\Textures\Skillpickbackground.dds)"},
            {Skillpicksheet, R"(..\Resources\Textures\Skillpicksheet.dds)"},
            {Spellsheet, R"(..\Resources\Textures\Spellsheet.dds)"},
            {WaveComplete, R"(..\Resources\Textures\WaveComplete.dds)"},
            {weaponsheet, R"(..\Resources\Textures\weaponsheet.dds)"}
        };
    }
}
