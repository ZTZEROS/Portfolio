#pragma once
#include <vector>
#include <map>
#include "AseFrame.h"
#include "MaterialTexture.h"


class AseLoader
{

public:
	AseLoader(void);
	~AseLoader(void);

	AseFrame* Load(char* szFilename);

private:
	const char* ASE_DIRECTORY = "./Data/ASE/";
	FILE* filePointer = nullptr;
	char token[1024];

	std::vector<MaterialTexture*> materialTextures;

	AseFrame* rootFrame = nullptr;
	std::map<std::string, AseFrame*> mapFrame;

	// 쉽게 토크나이징 하기 위한 유틸리티 클래스들
	char* GetToken();
	float GetFloat();
	int GetInteger();	
	bool IsWhite(char c);
	bool IsEqual(char* s1, char* s2);
	void SkipBlock();

	// 실제로 필요한 데이터 읽어들이는 부분
	void Process_MATERIAL_LIST();
	void Process_MATERIAL(OUT MaterialTexture* mt);
	void Process_MAP_DIFFUSE(OUT MaterialTexture* mt);
	void Process_GEOMOBJECT(OUT AseFrame* frame);	
	void Process_MESH(OUT AseFrame* frame);	
	void Process_MESH_VERTEX_LIST(OUT std::vector<D3DXVECTOR3>& posArray);
	void Process_MESH_FACE_LIST(IN std::vector<D3DXVECTOR3>& posArray, OUT std::vector<FVF_PositionNormalTexture>& vertex);
	void Process_MESH_TVERTLIST(OUT std::vector<D3DXVECTOR2>& uvArray);	
	void Process_ID_MESH_TFACELIST(IN std::vector<D3DXVECTOR2>& uvArray, OUT std::vector<FVF_PositionNormalTexture>& vertex);
	void Process_MESH_NORMALS(OUT std::vector<FVF_PositionNormalTexture>& vertex);
	void Process_NODE_TM(OUT AseFrame* frame);
	void Process_TM_ANIMATION(OUT AseFrame* frame);
	void Process_POS_TRACK(OUT AseFrame* frame);
	void Process_ROT_TRACK(OUT AseFrame* frame);
};


