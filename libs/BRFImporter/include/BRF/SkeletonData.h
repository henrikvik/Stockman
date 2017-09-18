#pragma once
#include <memory>
#include <vector>
#include <string>
#include "BRFImporterStructs.h"

namespace BRFImporterLib
{
	class FrameDataContainer
	{
	public:
		std::shared_ptr<JointCountHeader> animationJointData;
		std::vector<std::shared_ptr<FrameHeader>> frameData;

		//CON DECON
		FrameDataContainer(unsigned int animatedJointCount); //new på ptrs
		~FrameDataContainer();
	};

	class SkeletonContainer
	{
	public:
		std::shared_ptr<SkeletonHeader> skeletonData;
		std::unique_ptr<JointHeader[]> jointData;
		std::unique_ptr<AnimationHeader[]> animationData;
		std::vector<std::shared_ptr<FrameDataContainer>> frameDataContainer;

		//CON DECON
		SkeletonContainer(unsigned int jointCount, unsigned int animationCount);
		SkeletonContainer* GetSkeletonContainer() { return this; }
		void SetFrameDataContainer(unsigned int animationJointCount);
		SkeletonContainer();
		~SkeletonContainer();

	};
	class SkeletonData
	{
	protected:
		std::shared_ptr<SkeletonContainer> skeletonDataContainer;
	public:
		void setData(std::shared_ptr<SkeletonContainer> srcSkeletonData);
		AnimationHeader* GetAnimation(unsigned int id);
		unsigned int GetAnimationID(unsigned int id);
		SkeletonHeader* GetSkeletonData();
		
		//CON DECON
		SkeletonData();
		~SkeletonData();
	};
}