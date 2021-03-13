#pragma once
class HeightMap
{
public:
	HeightMap();
	~HeightMap();

	virtual void Initialize(char* mapFileName, char* textureFileName);
	virtual void Destroy();
	virtual void Render();

	bool GetHeight(float& outHeight, float x, float z);

	D3DXMATRIXA16 GetWorld() { return world; }
	LPDIRECT3DTEXTURE9 GetTexture() { return texture; }

	void SetSharpness(float sharpness) { Sharpness = sharpness; }

protected:
	const char* HEIGHTMAP_DIRECTORY = "../ResourceData/Texture/";

	int mapSize = -1;	//raw파일의 픽셀에 한변의 길이
	int tileCount = -1;	//mapSize-1
	int vertexCount = -1;	//mapSize*mapSize
	int triangleCount = -1;	//tileCount*tileCount*2

	int leftTopOfTerrain;
	float Sharpness = 0.1f;

	std::vector<D3DXVECTOR3> vertex;

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9 indexBuffer = nullptr;

	D3DMATERIAL9 material;
	LPDIRECT3DTEXTURE9 texture = nullptr;

	D3DXMATRIXA16 world;
};

