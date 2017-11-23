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
        
        Files toEnum(const char * str);
    }
    namespace Maps
    {
        enum Files
        {
            StockMan_LvL,
            Stock_Map_V1,
            Stock_Map_V1_Old,
            Stock_Map_V1_V2_old,
            Stock_Map_V1__,
            Test_Map
        };
        const std::map<Files, const char *> Paths =
        {
            {StockMan_LvL, R"(..\Resources\Maps\StockMan_LvL.toml)"},
            {Stock_Map_V1, R"(..\Resources\Maps\Stock_Map_V1.toml)"},
            {Stock_Map_V1_Old, R"(..\Resources\Maps\Stock_Map_V1_Old.toml)"},
            {Stock_Map_V1_V2_old, R"(..\Resources\Maps\Stock_Map_V1_V2_old.toml)"},
            {Stock_Map_V1__, R"(..\Resources\Maps\Stock_Map_V1__.toml)"},
            {Test_Map, R"(..\Resources\Maps\Test_Map.toml)"}
        };
        
        Files toEnum(const char * str);
    }
    namespace Models
    {
        enum Files
        {
            Ammocrystal,
            AmmoPackCrossBolt,
            AnimatedSummonUnit,
            AnimationTest,
            Barrel,
            Crate,
            Crossbow,
            Crossbowbolt,
            Crossbowbolts,
            Dead_Tree,
            Fence1,
            Fence2,
            FencePost,
            Fence_Post,
            Firepit,
            GrapplingPoint,
            Grappling_Point,
            Ground,
            Hammer,
            House1,
            House1old,
            House2,
            House3,
            Ice,
            Icecickle,
            Jumppad,
            Jump_Pad,
            Lamp_Post,
            Log,
            Lower_Ground,
            Market_Tent,
            MenuScene,
            Rock1,
            Rock2,
            Rock3,
            SkySphere,
            Small_Crate,
            Snow,
            SnowHeap,
            Snow_Heap,
            Staff,
            StaticSummon,
            Stone_Decoration,
            SummonUnitWithAnim,
            Table,
            Tablet1,
            Tee_Pee,
            Tree,
            UnitCube
        };
        const std::map<Files, const char *> Paths =
        {
            {Ammocrystal, R"(..\Resources\Models\Ammocrystal.hyb)"},
            {AmmoPackCrossBolt, R"(..\Resources\Models\AmmoPackCrossBolt.hyb)"},
            {AnimatedSummonUnit, R"(..\Resources\Models\AnimatedSummonUnit.hyb)"},
            {AnimationTest, R"(..\Resources\Models\AnimationTest.hyb)"},
            {Barrel, R"(..\Resources\Models\Barrel.hyb)"},
            {Crate, R"(..\Resources\Models\Crate.hyb)"},
            {Crossbow, R"(..\Resources\Models\Crossbow.hyb)"},
            {Crossbowbolt, R"(..\Resources\Models\Crossbowbolt.hyb)"},
            {Crossbowbolts, R"(..\Resources\Models\Crossbowbolts.hyb)"},
            {Dead_Tree, R"(..\Resources\Models\Dead_Tree.hyb)"},
            {Fence1, R"(..\Resources\Models\Fence1.hyb)"},
            {Fence2, R"(..\Resources\Models\Fence2.hyb)"},
            {FencePost, R"(..\Resources\Models\FencePost.hyb)"},
            {Fence_Post, R"(..\Resources\Models\Fence_Post.hyb)"},
            {Firepit, R"(..\Resources\Models\Firepit.hyb)"},
            {GrapplingPoint, R"(..\Resources\Models\GrapplingPoint.hyb)"},
            {Grappling_Point, R"(..\Resources\Models\Grappling_Point.hyb)"},
            {Ground, R"(..\Resources\Models\Ground.hyb)"},
            {Hammer, R"(..\Resources\Models\Hammer.hyb)"},
            {House1, R"(..\Resources\Models\House1.hyb)"},
            {House1old, R"(..\Resources\Models\House1old.hyb)"},
            {House2, R"(..\Resources\Models\House2.hyb)"},
            {House3, R"(..\Resources\Models\House3.hyb)"},
            {Ice, R"(..\Resources\Models\Ice.hyb)"},
            {Icecickle, R"(..\Resources\Models\Icecickle.hyb)"},
            {Jumppad, R"(..\Resources\Models\Jumppad.hyb)"},
            {Jump_Pad, R"(..\Resources\Models\Jump_Pad.hyb)"},
            {Lamp_Post, R"(..\Resources\Models\Lamp_Post.hyb)"},
            {Log, R"(..\Resources\Models\Log.hyb)"},
            {Lower_Ground, R"(..\Resources\Models\Lower_Ground.hyb)"},
            {Market_Tent, R"(..\Resources\Models\Market_Tent.hyb)"},
            {MenuScene, R"(..\Resources\Models\MenuScene.hyb)"},
            {Rock1, R"(..\Resources\Models\Rock1.hyb)"},
            {Rock2, R"(..\Resources\Models\Rock2.hyb)"},
            {Rock3, R"(..\Resources\Models\Rock3.hyb)"},
            {SkySphere, R"(..\Resources\Models\SkySphere.hyb)"},
            {Small_Crate, R"(..\Resources\Models\Small_Crate.hyb)"},
            {Snow, R"(..\Resources\Models\Snow.hyb)"},
            {SnowHeap, R"(..\Resources\Models\SnowHeap.hyb)"},
            {Snow_Heap, R"(..\Resources\Models\Snow_Heap.hyb)"},
            {Staff, R"(..\Resources\Models\Staff.hyb)"},
            {StaticSummon, R"(..\Resources\Models\StaticSummon.hyb)"},
            {Stone_Decoration, R"(..\Resources\Models\Stone_Decoration.hyb)"},
            {SummonUnitWithAnim, R"(..\Resources\Models\SummonUnitWithAnim.hyb)"},
            {Table, R"(..\Resources\Models\Table.hyb)"},
            {Tablet1, R"(..\Resources\Models\Tablet1.hyb)"},
            {Tee_Pee, R"(..\Resources\Models\Tee_Pee.hyb)"},
            {Tree, R"(..\Resources\Models\Tree.hyb)"},
            {UnitCube, R"(..\Resources\Models\UnitCube.hyb)"}
        };
        
        Files toEnum(const char * str);
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
        
        Files toEnum(const char * str);
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
        
        Files toEnum(const char * str);
    }
}
