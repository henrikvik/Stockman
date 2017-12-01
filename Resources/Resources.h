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
            KG26,
            nordic
        };
        const std::map<Files, const char *> Paths =
        {
            {KG14, R"(..\Resources\Fonts\KG14.spritefont)"},
            {KG18, R"(..\Resources\Fonts\KG18.spritefont)"},
            {KG26, R"(..\Resources\Fonts\KG26.spritefont)"},
            {nordic, R"(..\Resources\Fonts\nordic.spritefont)"}
        };
        
        Files toEnum(const char * str);
    }
    namespace Maps
    {
        enum Files
        {
            StockMan_LvL,
            Stock_Map,
            Stock_Map_,
            Stock_Map_1222,
            Stock_Map_V1_Old,
            Stock_Map_V1_V2_old,
            Stock_Map_V1__,
            Stock_Map_V1______,
            Stock_Map_V4,
            Stock_Map_V5,
            Test_Map
        };
        const std::map<Files, const char *> Paths =
        {
            {StockMan_LvL, R"(..\Resources\Maps\StockMan_LvL.toml)"},
            {Stock_Map, R"(..\Resources\Maps\Stock_Map.toml)"},
            {Stock_Map_, R"(..\Resources\Maps\Stock_Map_.toml)"},
            {Stock_Map_1222, R"(..\Resources\Maps\Stock_Map_1222.toml)"},
            {Stock_Map_V1_Old, R"(..\Resources\Maps\Stock_Map_V1_Old.toml)"},
            {Stock_Map_V1_V2_old, R"(..\Resources\Maps\Stock_Map_V1_V2_old.toml)"},
            {Stock_Map_V1__, R"(..\Resources\Maps\Stock_Map_V1__.toml)"},
            {Stock_Map_V1______, R"(..\Resources\Maps\Stock_Map_V1______.toml)"},
            {Stock_Map_V4, R"(..\Resources\Maps\Stock_Map_V4.toml)"},
            {Stock_Map_V5, R"(..\Resources\Maps\Stock_Map_V5.toml)"},
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
            AnimationTest,
            Barrel,
            Bridge,
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
            Grass,
            Grass1,
            Ground,
            Hammer,
            House1,
            House2,
            House3,
            Ice,
            Icecickle,
            Jump_Pad,
            Lamp_Post,
            Log,
            Lower_Island,
            Market_Tent,
            MenuScene,
            Necromancer,
            Rock1,
            Rock2,
            Rock3,
            SkySphere,
            Small_Crate,
            SnowHeap,
            Snow_,
            Snow_Heap,
            Snow__,
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
            {AnimationTest, R"(..\Resources\Models\AnimationTest.hyb)"},
            {Barrel, R"(..\Resources\Models\Barrel.hyb)"},
            {Bridge, R"(..\Resources\Models\Bridge.hyb)"},
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
            {Grass, R"(..\Resources\Models\Grass.hyb)"},
            {Grass1, R"(..\Resources\Models\Grass1.hyb)"},
            {Ground, R"(..\Resources\Models\Ground.hyb)"},
            {Hammer, R"(..\Resources\Models\Hammer.hyb)"},
            {House1, R"(..\Resources\Models\House1.hyb)"},
            {House2, R"(..\Resources\Models\House2.hyb)"},
            {House3, R"(..\Resources\Models\House3.hyb)"},
            {Ice, R"(..\Resources\Models\Ice.hyb)"},
            {Icecickle, R"(..\Resources\Models\Icecickle.hyb)"},
            {Jump_Pad, R"(..\Resources\Models\Jump_Pad.hyb)"},
            {Lamp_Post, R"(..\Resources\Models\Lamp_Post.hyb)"},
            {Log, R"(..\Resources\Models\Log.hyb)"},
            {Lower_Island, R"(..\Resources\Models\Lower_Island.hyb)"},
            {Market_Tent, R"(..\Resources\Models\Market_Tent.hyb)"},
            {MenuScene, R"(..\Resources\Models\MenuScene.hyb)"},
            {Necromancer, R"(..\Resources\Models\Necromancer.hyb)"},
            {Rock1, R"(..\Resources\Models\Rock1.hyb)"},
            {Rock2, R"(..\Resources\Models\Rock2.hyb)"},
            {Rock3, R"(..\Resources\Models\Rock3.hyb)"},
            {SkySphere, R"(..\Resources\Models\SkySphere.hyb)"},
            {Small_Crate, R"(..\Resources\Models\Small_Crate.hyb)"},
            {SnowHeap, R"(..\Resources\Models\SnowHeap.hyb)"},
            {Snow_, R"(..\Resources\Models\Snow_.hyb)"},
            {Snow_Heap, R"(..\Resources\Models\Snow_Heap.hyb)"},
            {Snow__, R"(..\Resources\Models\Snow__.hyb)"},
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
            KawaseDualFilterDownsample,
            KawaseDualFilterUpsample,
            KawaseDualFilterVS,
            LightGridCulling,
            Moon,
            ParticlePlane,
            PostFX,
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
            {KawaseDualFilterDownsample, R"(..\Resources\Shaders\KawaseDualFilterDownsample.hlsl)"},
            {KawaseDualFilterUpsample, R"(..\Resources\Shaders\KawaseDualFilterUpsample.hlsl)"},
            {KawaseDualFilterVS, R"(..\Resources\Shaders\KawaseDualFilterVS.hlsl)"},
            {LightGridCulling, R"(..\Resources\Shaders\LightGridCulling.hlsl)"},
            {Moon, R"(..\Resources\Shaders\Moon.hlsl)"},
            {ParticlePlane, R"(..\Resources\Shaders\ParticlePlane.hlsl)"},
            {PostFX, R"(..\Resources\Shaders\PostFX.hlsl)"},
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
            CardBackground,
            Crosshair,
            Diffusemoon,
            Gameover,
            Gamesheet,
            Grid,
            Highscore,
            Highscoretext,
            iconsheet,
            IntroScreen,
            Loadingscreen,
            MainmenuClean,
            Mainmenutext,
            MissingTexture,
            particle,
            Pausetext,
            randomNormals,
            Settings,
            SettingsButtons,
            Skillpickbackground,
            Skillpicksheet,
            Spellsheet,
            TextSkillPick,
            WaveComplete,
            weaponsheet
        };
        const std::map<Files, const char *> Paths =
        {
            {CardBackground, R"(..\Resources\Textures\CardBackground.dds)"},
            {Crosshair, R"(..\Resources\Textures\Crosshair.dds)"},
            {Diffusemoon, R"(..\Resources\Textures\Diffusemoon.dds)"},
            {Gameover, R"(..\Resources\Textures\Gameover.dds)"},
            {Gamesheet, R"(..\Resources\Textures\Gamesheet.dds)"},
            {Grid, R"(..\Resources\Textures\Grid.png)"},
            {Highscore, R"(..\Resources\Textures\Highscore.dds)"},
            {Highscoretext, R"(..\Resources\Textures\Highscoretext.dds)"},
            {iconsheet, R"(..\Resources\Textures\iconsheet.dds)"},
            {IntroScreen, R"(..\Resources\Textures\IntroScreen.dds)"},
            {Loadingscreen, R"(..\Resources\Textures\Loadingscreen.dds)"},
            {MainmenuClean, R"(..\Resources\Textures\MainmenuClean.dds)"},
            {Mainmenutext, R"(..\Resources\Textures\Mainmenutext.dds)"},
            {MissingTexture, R"(..\Resources\Textures\MissingTexture.dds)"},
            {particle, R"(..\Resources\Textures\particle.png)"},
            {Pausetext, R"(..\Resources\Textures\Pausetext.dds)"},
            {randomNormals, R"(..\Resources\Textures\randomNormals.dds)"},
            {Settings, R"(..\Resources\Textures\Settings.dds)"},
            {SettingsButtons, R"(..\Resources\Textures\SettingsButtons.dds)"},
            {Skillpickbackground, R"(..\Resources\Textures\Skillpickbackground.dds)"},
            {Skillpicksheet, R"(..\Resources\Textures\Skillpicksheet.dds)"},
            {Spellsheet, R"(..\Resources\Textures\Spellsheet.dds)"},
            {TextSkillPick, R"(..\Resources\Textures\TextSkillPick.dds)"},
            {WaveComplete, R"(..\Resources\Textures\WaveComplete.dds)"},
            {weaponsheet, R"(..\Resources\Textures\weaponsheet.dds)"}
        };
        
        Files toEnum(const char * str);
    }
}
