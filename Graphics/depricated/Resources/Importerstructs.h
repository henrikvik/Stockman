#pragma once

namespace BRFImporterLib
{
	//STRUCTS WITH #PRAGMA REGION IN ORDER: 1. STATIC 2. DYNAMICS OF THAT STATIC, REPEAT
#pragma region Mainheader
	struct MainHeader
	{
		bool lights;

		unsigned int meshAmount;
		unsigned int materialAmount;
		unsigned int cameraAmount;
		unsigned int skeletonAmount;
		unsigned int morphAnimAmount;
		unsigned int groupAmount;
		unsigned int attributeAmount;
	};
#pragma endregion
#pragma region MeshHeader
	struct MeshHeader
	{
		char meshName[256];
		unsigned int objectID;
		unsigned int parentID;		// 0 = Default

		unsigned int vertexCount;
		unsigned int indexCount;
		unsigned int materialID;

		unsigned int attrCount = 0;		// 0 = Default

		bool boundingBox = false;
		bool hasSkeleton = false;

		double translation[3];
		double rotation[3];
		double scale[3];
	};
#pragma endregion
#pragma region MeshHeader Dynamics
	struct OOBBHeader
	{
		double extents[3];
		double orientation[3];

	};
	struct IndexHeader
	{
		unsigned int vertIndex;
	};
	struct VertexHeaderNoSkeleton
	{
		double pos[3];
		double normal[3];
		double uv[2];
		double tangent[2];
		double biTangent[2];
	};
	struct VertexHeader
	{
		double pos[3];
		double normal[3];
		double uv[2];
		double tangent[2];
		double biTangent[2];
		//unsigned int weightAmmount[4];
	};
	struct WeigthsHeader
	{
		double influence;
		unsigned int jointID;
	};
#pragma endregion
#pragma region MaterialHeader
	struct MaterialHeader
	{
		unsigned int Id;

		char matName[256];

		double diffuseVal[3];
		double specularVal[3];

		char diffMap[256];
		char specMap[256];
		char normalMap[256];
		char reflectMap[256]; //Was Gloss
		char glowMap[256];
	};
#pragma endregion
#pragma region SkeletonHeader
	struct SkeletonHeader
	{
		unsigned int skeletonID;
		unsigned int jointCount;
		unsigned int animationCount;
	};
#pragma endregion
#pragma region SkeletonHeader Dynamics
	struct JointHeader
	{
		char jointName[256];
		unsigned int jointID;
		unsigned int ParentJointID;		// 0 = Default;
		double bindPoseMatrix[4][4];
		double pos[3];
		double rotation[3];
	};
	struct AnimationHeader
	{
		char animationName[256];
		unsigned int jointCount;
		unsigned int animationID;
	};
	struct JointCountHeader
	{
		unsigned int frameCount;
		unsigned int jointID;
	};
	struct FrameHeader
	{
		unsigned int frameID;
		double frameMatrix[4][4];
	};
#pragma endregion
#pragma region CameraHeader
	struct CameraHeader
	{
		unsigned int objectID;
		unsigned int parentID;		// 0 = Default

		char camName[256];

		double position[3];
		double rotation[3];
	};
#pragma endregion
#pragma region LightHeader
	struct LightHeader
	{
		unsigned int spotCount;
		unsigned int areaCount;
		unsigned int pointCount;
		unsigned int directionalCount;

	};
#pragma endregion
#pragma region LightHeader Dynamics
	struct SpotLightHeader
	{
		unsigned int objectID;
		unsigned int parentID;		// 0 = Default
		double pos[3];
		double rot[3];
		double scale[3];
		double spotRadius;
		double color[3];
		double intensity;
		double lookAtLH[3];//new
		double lightRange;//new
		double attenuation;//new
	};
	struct AreaLightHeader
	{
		unsigned int objectID;
		unsigned int parentID;		// 0 = Default
		double pos[3];
		double rot[3];
		double scale[3];
		double color[3];
		double intensity;
	};
	struct PointLightHeader
	{
		unsigned int objectID;
		unsigned int parentID;		// 0 = Default
		double pos[3];
		double color[3];
		double intensity;
		double lookAtLH[3];//new
		double lightRange;//new
		double attenuation;//new
	};
	struct DirLightHeader
	{
		unsigned int objectID;
		unsigned int parentID;		// 0 = Default
		double pos[3];
		double rot[3];
		double color[3];
		double intensity;
		double lookAtLH[3];//new
	};
#pragma endregion
#pragma region GroupHeader
	struct GroupHeader
	{
		char groupName[256];
		unsigned int objectID;		// 0 = Default	
		unsigned int parentID;		// 0 = Default

		double translation[3];
		double rotation[3];
		double scale[3];
	};
#pragma endregion

#pragma region AttributeHeader
	struct AttributesHeader
	{

		unsigned int floatAmount;
		unsigned int intAmount;
		unsigned int boolAmount;
		unsigned int stringAmount;
		unsigned int vectorAmount;
	};
#pragma endregion
#pragma region AttributeHeader Dynamics
	struct VectorAttrHeader
	{
		char attrName[256];

		float value[3];


	};

	struct StringAttrHeader
	{
		char attrName[256];
		char value[256];

	};

	struct FloatAttrHeader
	{
		char attrName[256];
		float value;
		float min;
		float max;

	};

	struct BoolAttrHeader
	{
		char attrName[256];
		bool value;
	};

	struct IntAttrHeader
	{
		char attrName[256];

		unsigned int value;

		unsigned int min;
		unsigned int max;
	};


#pragma endregion
}

