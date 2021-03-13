#pragma once
#include <vector>
#include <map>
#include "../FVF.h"
#include "MaterialTexture.h"
#include "ObjFileGroup.h"


class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();

	static void LoadObjFileData(OUT std::vector<ObjFileGroup*>& group, char* fileName);

	static void LoadObj_OnlyVertex(OUT std::vector<D3DXVECTOR3>& vertex, char* fileName);

private:

	static const char* OBJ_DIRECTORY;
	static void LoadMtlFileData(OUT std::map<std::string, MaterialTexture*>& materialTextures, char* fileName);
	
};

