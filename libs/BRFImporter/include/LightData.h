#pragma once
#include <memory>
#include "BRFImporterStructs.h"

namespace BRFImporterLib
{

	class LightContainer
	{
	public:
		std::shared_ptr<LightHeader> lightData;
		std::unique_ptr<SpotLightHeader[]> spotLightData;
		std::unique_ptr<AreaLightHeader[]> areaLightData;
		std::unique_ptr<PointLightHeader[]> pointLightData;
		std::unique_ptr<DirLightHeader[]> dirLightData;
		LightContainer(
			unsigned int spotCount,
			unsigned int areaCount,
			unsigned int pointCount,
			unsigned int dirCount
		);
		~LightContainer();
	};

	class LightData
	{
	protected:
		std::shared_ptr<LightContainer> lightDataContainer;
	public:
		void SetData(std::shared_ptr<LightContainer> SrcMeshData);

		LightHeader* GetLightData();
		SpotLightHeader GetSpotLightData(unsigned int iD);
		AreaLightHeader GetAreaLightData(unsigned int iD);
		PointLightHeader GetPointLightData(unsigned int iD);
		DirLightHeader GetDirLightData(unsigned int iD);

		//CON DECON
		LightData();
		~LightData();
	};
}