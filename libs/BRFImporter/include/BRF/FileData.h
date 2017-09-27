#pragma once
#include <string>
#include <fstream>
#include "Fetch.h"

namespace BRFImporterLib
{
	class FileData
	{
	protected:
		//funcBlock
		std::vector<std::shared_ptr<MeshData>> LoadMesh(std::shared_ptr<MainHeader> tempMain, std::ifstream *inFile);
		std::shared_ptr<MaterialData> LoadMaterial(std::shared_ptr<MainHeader> tempMain, std::ifstream *inFile);
		std::vector<std::shared_ptr<SkeletonData>> LoadSkeleton(std::shared_ptr<MainHeader> tempMain, std::ifstream *inFile);
		std::shared_ptr<LightData> LoadLight(std::shared_ptr<MainHeader> tempMain, std::ifstream *inFile);
		std::vector<std::shared_ptr<MorphData>> LoadMorph(std::shared_ptr<MainHeader> tempMain, std::ifstream * inFile);
		std::vector<std::shared_ptr<GroupData>> LoadGroups(std::shared_ptr<MainHeader>tempMain, std::ifstream*inFile);
		std::vector<std::shared_ptr<CameraData>> LoadCameras(std::shared_ptr<MainHeader>tempMain, std::ifstream*inFile);
	public:
		std::shared_ptr<Fetch> fetch;

		//takes a string filename and what specific parts of the file to read.
		void LoadFile(std::string fileName, bool mesh, bool skeleton, bool material);

		//CON DECON
		FileData();
		~FileData();
	};
}