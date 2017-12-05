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
            Stock_Map
        };
        const std::map<Files, const char *> Paths =
        {
            {Stock_Map, R"(..\Resources\Maps\Stock_Map.toml)"}
        };
        
        Files toEnum(const char * str);
        namespace OLD
        {
            enum Files
            {
                Stock_Map_V6,
                Test_Map
            };
            const std::map<Files, const char *> Paths =
            {
                {Stock_Map_V6, R"(..\Resources\Maps\OLD\Stock_Map_V6.toml)"},
                {Test_Map, R"(..\Resources\Maps\OLD\Test_Map.toml)"}
            };
            
            Files toEnum(const char * str);
        }
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
            Border_Walls,
            Bridge,
            Crate,
            Crossbow,
            Crossbowbolt,
            Crossbowbolts,
            Dead_Tree,
            Docks,
            Fence1,
            Fence2,
            Fence_Post,
            Firepit,
            Grappling_Point,
            Ground,
            Ground_Old,
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
            Mountain,
            Necromancer,
            Rock1,
            Rock2,
            Rock3,
            SkySphere,
            Small_Crate,
            Snow,
            Snow_Heap,
            Spear,
            Staff,
            StaticSummon,
            Stone_Decoration,
            SummonUnitWithAnim,
            Table,
            Tablet1,
            Tee_Pee,
            Tomb_Stone,
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
            {Border_Walls, R"(..\Resources\Models\Border_Walls.hyb)"},
            {Bridge, R"(..\Resources\Models\Bridge.hyb)"},
            {Crate, R"(..\Resources\Models\Crate.hyb)"},
            {Crossbow, R"(..\Resources\Models\Crossbow.hyb)"},
            {Crossbowbolt, R"(..\Resources\Models\Crossbowbolt.hyb)"},
            {Crossbowbolts, R"(..\Resources\Models\Crossbowbolts.hyb)"},
            {Dead_Tree, R"(..\Resources\Models\Dead_Tree.hyb)"},
            {Docks, R"(..\Resources\Models\Docks.hyb)"},
            {Fence1, R"(..\Resources\Models\Fence1.hyb)"},
            {Fence2, R"(..\Resources\Models\Fence2.hyb)"},
            {Fence_Post, R"(..\Resources\Models\Fence_Post.hyb)"},
            {Firepit, R"(..\Resources\Models\Firepit.hyb)"},
            {Grappling_Point, R"(..\Resources\Models\Grappling_Point.hyb)"},
            {Ground, R"(..\Resources\Models\Ground.hyb)"},
            {Ground_Old, R"(..\Resources\Models\Ground_Old.hyb)"},
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
            {Mountain, R"(..\Resources\Models\Mountain.hyb)"},
            {Necromancer, R"(..\Resources\Models\Necromancer.hyb)"},
            {Rock1, R"(..\Resources\Models\Rock1.hyb)"},
            {Rock2, R"(..\Resources\Models\Rock2.hyb)"},
            {Rock3, R"(..\Resources\Models\Rock3.hyb)"},
            {SkySphere, R"(..\Resources\Models\SkySphere.hyb)"},
            {Small_Crate, R"(..\Resources\Models\Small_Crate.hyb)"},
            {Snow, R"(..\Resources\Models\Snow.hyb)"},
            {Snow_Heap, R"(..\Resources\Models\Snow_Heap.hyb)"},
            {Spear, R"(..\Resources\Models\Spear.hyb)"},
            {Staff, R"(..\Resources\Models\Staff.hyb)"},
            {StaticSummon, R"(..\Resources\Models\StaticSummon.hyb)"},
            {Stone_Decoration, R"(..\Resources\Models\Stone_Decoration.hyb)"},
            {SummonUnitWithAnim, R"(..\Resources\Models\SummonUnitWithAnim.hyb)"},
            {Table, R"(..\Resources\Models\Table.hyb)"},
            {Tablet1, R"(..\Resources\Models\Tablet1.hyb)"},
            {Tee_Pee, R"(..\Resources\Models\Tee_Pee.hyb)"},
            {Tomb_Stone, R"(..\Resources\Models\Tomb_Stone.hyb)"},
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
            controlscheme,
            Crosshair,
            Diffusemoon,
            Enraged,
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
            Retrytextsheet,
            Settings,
            SettingsButtons,
            Skillpickbackground,
            Skillpicksheet,
            Spellsheet,
            TextSkillPick,
            wavebegins,
            WaveComplete,
            weaponsheet
        };
        const std::map<Files, const char *> Paths =
        {
            {CardBackground, R"(..\Resources\Textures\CardBackground.dds)"},
            {controlscheme, R"(..\Resources\Textures\controlscheme.dds)"},
            {Crosshair, R"(..\Resources\Textures\Crosshair.dds)"},
            {Diffusemoon, R"(..\Resources\Textures\Diffusemoon.dds)"},
            {Enraged, R"(..\Resources\Textures\Enraged.dds)"},
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
            {Retrytextsheet, R"(..\Resources\Textures\Retrytextsheet.dds)"},
            {Settings, R"(..\Resources\Textures\Settings.dds)"},
            {SettingsButtons, R"(..\Resources\Textures\SettingsButtons.dds)"},
            {Skillpickbackground, R"(..\Resources\Textures\Skillpickbackground.dds)"},
            {Skillpicksheet, R"(..\Resources\Textures\Skillpicksheet.dds)"},
            {Spellsheet, R"(..\Resources\Textures\Spellsheet.dds)"},
            {TextSkillPick, R"(..\Resources\Textures\TextSkillPick.dds)"},
            {wavebegins, R"(..\Resources\Textures\wavebegins.dds)"},
            {WaveComplete, R"(..\Resources\Textures\WaveComplete.dds)"},
            {weaponsheet, R"(..\Resources\Textures\weaponsheet.dds)"}
        };
        
        Files toEnum(const char * str);
    }
}
