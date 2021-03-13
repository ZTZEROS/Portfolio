#pragma once
#include "../Mesh/SkinnedMesh.h"

// X파일에서 불러온 원본 데이터 관리를 위한 메니저
class SkinnedMeshManager
{
private:
	SkinnedMeshManager();
	SkinnedMeshManager(const SkinnedMeshManager& other);
	~SkinnedMeshManager();

public:
	static SkinnedMeshManager& Get()
	{
		static SkinnedMeshManager instance;
		return instance;
	}

	void Destroy();

	SkinnedMesh* GetOriginal(char* path, char* fileName);

private:
	const char* XFILE_DIRECTORY = "./XFile/";

	std::map<std::string, SkinnedMesh*	> orignalSkinnedMeshMap;
};

