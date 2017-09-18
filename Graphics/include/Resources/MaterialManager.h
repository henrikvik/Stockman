#pragma once
#include <vector>
#include "../Datatypes.h"

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();
	void initialize(ID3D11Device* gDevice);
	void release();


private:
	std::vector<Material>* materials;
};

