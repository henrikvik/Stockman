#include "Resources.h"
namespace Resources
{
    namespace Fonts
    {
        Files toEnum(const char * str)
        {
            if (strcmp(str, "KG14") == 0) return KG14;
            if (strcmp(str, "KG18") == 0) return KG18;
            if (strcmp(str, "KG26") == 0) return KG26;
            if (strcmp(str, "nordic") == 0) return nordic;
            throw "Could not find matching enum.";
        }
    }
    namespace Maps
    {
        Files toEnum(const char * str)
        {
            if (strcmp(str, "StockMan_LvL") == 0) return StockMan_LvL;
            if (strcmp(str, "Stock_Map") == 0) return Stock_Map;
            if (strcmp(str, "Stock_Map_") == 0) return Stock_Map_;
            if (strcmp(str, "Stock_Map_1222") == 0) return Stock_Map_1222;
            if (strcmp(str, "Stock_Map_V1_Old") == 0) return Stock_Map_V1_Old;
            if (strcmp(str, "Stock_Map_V1_V2_old") == 0) return Stock_Map_V1_V2_old;
            if (strcmp(str, "Stock_Map_V1__") == 0) return Stock_Map_V1__;
            if (strcmp(str, "Stock_Map_V1______") == 0) return Stock_Map_V1______;
            if (strcmp(str, "Stock_Map_V4") == 0) return Stock_Map_V4;
            if (strcmp(str, "Stock_Map_V5") == 0) return Stock_Map_V5;
            if (strcmp(str, "Test_Map") == 0) return Test_Map;
            throw "Could not find matching enum.";
        }
    }
    namespace Models
    {
        Files toEnum(const char * str)
        {
            if (strcmp(str, "Ammocrystal") == 0) return Ammocrystal;
            if (strcmp(str, "AmmoPackCrossBolt") == 0) return AmmoPackCrossBolt;
            if (strcmp(str, "AnimatedSummonUnit") == 0) return AnimatedSummonUnit;
            if (strcmp(str, "AnimationTest") == 0) return AnimationTest;
            if (strcmp(str, "Barrel") == 0) return Barrel;
            if (strcmp(str, "Bridge") == 0) return Bridge;
            if (strcmp(str, "Crate") == 0) return Crate;
            if (strcmp(str, "Crossbow") == 0) return Crossbow;
            if (strcmp(str, "Crossbowbolt") == 0) return Crossbowbolt;
            if (strcmp(str, "Crossbowbolts") == 0) return Crossbowbolts;
            if (strcmp(str, "Dead_Tree") == 0) return Dead_Tree;
            if (strcmp(str, "Fence1") == 0) return Fence1;
            if (strcmp(str, "Fence2") == 0) return Fence2;
            if (strcmp(str, "FencePost") == 0) return FencePost;
            if (strcmp(str, "Fence_Post") == 0) return Fence_Post;
            if (strcmp(str, "Firepit") == 0) return Firepit;
            if (strcmp(str, "GrapplingPoint") == 0) return GrapplingPoint;
            if (strcmp(str, "Grappling_Point") == 0) return Grappling_Point;
            if (strcmp(str, "Grass") == 0) return Grass;
            if (strcmp(str, "Grass1") == 0) return Grass1;
            if (strcmp(str, "Ground") == 0) return Ground;
            if (strcmp(str, "Hammer") == 0) return Hammer;
            if (strcmp(str, "House1") == 0) return House1;
            if (strcmp(str, "House1old") == 0) return House1old;
            if (strcmp(str, "House2") == 0) return House2;
            if (strcmp(str, "House3") == 0) return House3;
            if (strcmp(str, "Ice") == 0) return Ice;
            if (strcmp(str, "Icecickle") == 0) return Icecickle;
            if (strcmp(str, "Jumppad") == 0) return Jumppad;
            if (strcmp(str, "Jump_Pad") == 0) return Jump_Pad;
            if (strcmp(str, "Lamp_Post") == 0) return Lamp_Post;
            if (strcmp(str, "Log") == 0) return Log;
            if (strcmp(str, "Lower_Island") == 0) return Lower_Island;
            if (strcmp(str, "Market_Tent") == 0) return Market_Tent;
            if (strcmp(str, "MenuScene") == 0) return MenuScene;
            if (strcmp(str, "Necromancer") == 0) return Necromancer;
            if (strcmp(str, "Rock1") == 0) return Rock1;
            if (strcmp(str, "Rock2") == 0) return Rock2;
            if (strcmp(str, "Rock3") == 0) return Rock3;
            if (strcmp(str, "SkySphere") == 0) return SkySphere;
            if (strcmp(str, "Small_Crate") == 0) return Small_Crate;
            if (strcmp(str, "SnowHeap") == 0) return SnowHeap;
            if (strcmp(str, "Snow_") == 0) return Snow_;
            if (strcmp(str, "Snow_Heap") == 0) return Snow_Heap;
            if (strcmp(str, "Snow__") == 0) return Snow__;
            if (strcmp(str, "Staff") == 0) return Staff;
            if (strcmp(str, "StaticSummon") == 0) return StaticSummon;
            if (strcmp(str, "Stone_Decoration") == 0) return Stone_Decoration;
            if (strcmp(str, "SummonUnitWithAnim") == 0) return SummonUnitWithAnim;
            if (strcmp(str, "Table") == 0) return Table;
            if (strcmp(str, "Tablet1") == 0) return Tablet1;
            if (strcmp(str, "Tee_Pee") == 0) return Tee_Pee;
            if (strcmp(str, "Tree") == 0) return Tree;
            if (strcmp(str, "UnitCube") == 0) return UnitCube;
            throw "Could not find matching enum.";
        }
    }
    namespace Shaders
    {
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
            if (strcmp(str, "ForwardPlus_VS_Animated") == 0) return ForwardPlus_VS_Animated;
            if (strcmp(str, "ForwardPlus_VS_Foliage") == 0) return ForwardPlus_VS_Foliage;
            if (strcmp(str, "ForwardPlus_VS_Static") == 0) return ForwardPlus_VS_Static;
            if (strcmp(str, "FullscreenQuad") == 0) return FullscreenQuad;
            if (strcmp(str, "GlowBlurHorizontal") == 0) return GlowBlurHorizontal;
            if (strcmp(str, "GlowBlurVertical") == 0) return GlowBlurVertical;
            if (strcmp(str, "GlowDownSampler") == 0) return GlowDownSampler;
            if (strcmp(str, "GlowMerger") == 0) return GlowMerger;
            if (strcmp(str, "GlowMipCombinder") == 0) return GlowMipCombinder;
            if (strcmp(str, "LightGridCulling") == 0) return LightGridCulling;
            if (strcmp(str, "Moon") == 0) return Moon;
            if (strcmp(str, "ParticlePlane") == 0) return ParticlePlane;
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
        Files toEnum(const char * str)
        {
            if (strcmp(str, "CardBackground") == 0) return CardBackground;
            if (strcmp(str, "Crosshair") == 0) return Crosshair;
            if (strcmp(str, "Diffusemoon") == 0) return Diffusemoon;
            if (strcmp(str, "Gameover") == 0) return Gameover;
            if (strcmp(str, "Gamesheet") == 0) return Gamesheet;
            if (strcmp(str, "Grid") == 0) return Grid;
            if (strcmp(str, "Highscore") == 0) return Highscore;
            if (strcmp(str, "Highscoretext") == 0) return Highscoretext;
            if (strcmp(str, "iconsheet") == 0) return iconsheet;
            if (strcmp(str, "IntroScreen") == 0) return IntroScreen;
            if (strcmp(str, "Loadingscreen") == 0) return Loadingscreen;
            if (strcmp(str, "MainmenuClean") == 0) return MainmenuClean;
            if (strcmp(str, "Mainmenutext") == 0) return Mainmenutext;
            if (strcmp(str, "MissingTexture") == 0) return MissingTexture;
            if (strcmp(str, "Pausetext") == 0) return Pausetext;
            if (strcmp(str, "randomNormals") == 0) return randomNormals;
            if (strcmp(str, "Settings") == 0) return Settings;
            if (strcmp(str, "SettingsButtons") == 0) return SettingsButtons;
            if (strcmp(str, "Skillpickbackground") == 0) return Skillpickbackground;
            if (strcmp(str, "Skillpicksheet") == 0) return Skillpicksheet;
            if (strcmp(str, "Spellsheet") == 0) return Spellsheet;
            if (strcmp(str, "TextSkillPick") == 0) return TextSkillPick;
            if (strcmp(str, "WaveComplete") == 0) return WaveComplete;
            if (strcmp(str, "weaponsheet") == 0) return weaponsheet;
            throw "Could not find matching enum.";
        }
    }
}
