#pragma once
#include "MeshData.h"
#include "MaterialData.h"
#include "LightData.h"
#include "SkeletonData.h"
#include "MorphData.h"
#include "GroupData.h"
#include "CameraData.h"

namespace BRFImporterLib
{
	class FetchContainer
	{
	private:
		std::shared_ptr<MainHeader> mainData;
		std::vector<std::shared_ptr<MeshData>> meshes;
		std::shared_ptr<MaterialData> materialData;
		std::vector<std::shared_ptr<SkeletonData>> skeletons;
		std::shared_ptr<LightData> lights;
		std::vector<std::shared_ptr<MorphData>> morphAnimations;
		std::vector<std::shared_ptr<GroupData>> groups;
		std::vector < std::shared_ptr<CameraData>> cameras;
	public:
		MainHeader* getMain();
		MeshHeader* GetMeshHeader(unsigned int meshID);
		MeshData* GetMesh(unsigned int meshID);
		MaterialContainer* GetMaterial(unsigned int materialID);
		SkeletonData* GetSkeleton(unsigned int skeletonID);
		LightData* GetLight();
		MorphData* GetMorphAnimation(unsigned int morphAnimationID);
		GroupData* GetGroup(unsigned int groupID);
		CameraData* GetCamera(unsigned int cameraID);
		//CONDECON
		FetchContainer(
			std::shared_ptr<MainHeader> mainData,
			std::vector<std::shared_ptr<MeshData>> meshVector,
			std::shared_ptr<MaterialData> materialData,
			std::vector<std::shared_ptr<SkeletonData>> skeletonVector,
			std::shared_ptr<LightData> lights,
			std::vector<std::shared_ptr<MorphData>> morphVector,
			std::vector<std::shared_ptr<GroupData>> groupVector,
			std::vector<std::shared_ptr<CameraData>> cameraVector
			);
		~FetchContainer();
	};

	class Fetch
	{
	
	private:
		std::shared_ptr<FetchContainer> FetchDataContainer;
	public:
		MainHeader* Main();
		MeshData* Mesh(unsigned int meshID);
		MaterialContainer* Material(unsigned int materialID);
		SkeletonData* Skeleton(unsigned int skeletonID);
		AnimationHeader* Animation(unsigned int skeletonID, unsigned int animationID);
		LightData* Light();
		MorphData* MorphAnimation(unsigned int morphAnimationID);
		GroupData* group(unsigned int groups);
		CameraData* camera(unsigned int groups);

		//CON DECON
		Fetch(std::shared_ptr<FetchContainer> fileFetch);

		Fetch();
		~Fetch();

	};
}
