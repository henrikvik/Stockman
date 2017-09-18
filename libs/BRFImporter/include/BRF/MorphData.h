#pragma once
#include <memory>
#include <vector>
#include "BRFImporterStructs.h"
namespace BRFImporterLib
{
	struct MorphDataContainer
	{
		std::shared_ptr<MorphAnimHeader> morphData;
		std::unique_ptr<MorphAnimKeyFrameHeader[]> morphKeyFrameData;
		std::unique_ptr<std::vector<MorphVertexHeader>[]> morphVertexData;

		MorphDataContainer(unsigned int numberOfKeyFrames, unsigned int vertPerShape);
	};

	class MorphData
	{
	private:
		std::shared_ptr<MorphDataContainer> morphDataContainer;
	public:
		MorphData();
		~MorphData();

		//future gets sets etc
		MorphAnimHeader* getMorphAnimationHeader() { return this->morphDataContainer->morphData.get(); };
		MorphAnimKeyFrameHeader getMorphAnimKeyFrame(unsigned int id) { return this->morphDataContainer->morphKeyFrameData.get()[id]; };
		std::vector<MorphVertexHeader> getMorphVertexHeader(unsigned int id) { return this->morphDataContainer->morphVertexData.get()[id]; };
		std::vector<MorphVertexHeader>* getMorphVertexHeaderVector(unsigned int vectorID) { return &this->morphDataContainer->morphVertexData[vectorID]; };
		void setData(std::shared_ptr<MorphDataContainer> srcMorphData);
	};
}
