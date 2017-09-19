#pragma once
#include <memory>
#include <vector>
#include <string>
#include "BRFImporterStructs.h"

namespace BRFImporterLib
{
	struct MaterialContainer
	{
		unsigned int Id;

		std::string matName;

		double diffuseVal[3];
		double specularVal[3];

		std::string diffMap;
		std::string specMap;
		std::string normalMap;
		std::string reflectMap; //Was Gloss
		std::string glowMap;
	};

	class MaterialData
	{
	protected:
		std::vector<std::shared_ptr<MaterialContainer>> materialContainer;
	public:
		void SetData(std::shared_ptr<MaterialContainer> SrcMaterialData);
		MaterialContainer* GetMaterialData(unsigned int materialID);
		MaterialData();
		~MaterialData();
	};
}

