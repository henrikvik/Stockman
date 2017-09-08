#pragma once
#include <Includes.h>
#include <Datatypes.h>

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();
	void initialize(ID3D11Device* gDevice);
	void release();


private:
	vector<Material>* materials;
};

