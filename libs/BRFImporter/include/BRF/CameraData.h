#pragma once

#include <memory>
#include "BRFImporterStructs.h"
namespace BRFImporterLib
{
	
	class CameraContainer
	{
	public:
		std::shared_ptr <CameraHeader> cameraData;		    //Group information
		CameraHeader * getCameraData();
		CameraContainer();
		~CameraContainer();
	};

	class CameraData
	{
	protected:
		std::shared_ptr<CameraContainer> cameraDataContainer;
	public:
		CameraHeader* getCameraData();
		void SetData(std::shared_ptr<CameraContainer> source);
		CameraData();
		~CameraData();
	};

}
