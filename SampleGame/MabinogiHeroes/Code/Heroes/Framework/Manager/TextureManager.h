#pragma once

class TextureManager
{
private:
	TextureManager();
	TextureManager(const TextureManager& other);
	~TextureManager();

public:
	static TextureManager& Get()
	{
		static TextureManager instance;
		return instance;
	}

	void Destroy();

	static inline LPDIRECT3DTEXTURE9 GetTexture(const char* filename, D3DXIMAGE_INFO* imageInfo = nullptr) { return TextureManager::Get().GetInstanceTexture(filename, imageInfo); }
	static inline LPDIRECT3DTEXTURE9 GetTexture(const char* path, const char* fileName) { return TextureManager::Get().GetInstanceTexture(path, fileName); }
	static inline LPDIRECT3DTEXTURE9 GetTexturePath(const char* path, const char* filename, D3DXIMAGE_INFO* imageInfo = nullptr) { return TextureManager::Get().GetInstanceTexturePath(path, filename, imageInfo); }
	static inline LPDIRECT3DTEXTURE9 GetTexturePathSolo(const char* path, const char* filename, D3DXIMAGE_INFO* imageInfo = nullptr) { return TextureManager::Get().GetInstanceTexturePathSolo(path, filename, imageInfo); }

private:
	const char* TEXTURE_DIRECTORY = "../ResourceData/Texture/";
	LPDIRECT3DTEXTURE9 temp;
	D3DXIMAGE_INFO tempInfo;
	//std::map<std::string, LPDIRECT3DTEXTURE9> textureMap;
	std::map<std::string, std::pair<LPDIRECT3DTEXTURE9, D3DXIMAGE_INFO>> textureDataMap;
	LPDIRECT3DTEXTURE9 GetInstanceTexture(const char* filename, D3DXIMAGE_INFO* imageInfo = nullptr);

	LPDIRECT3DTEXTURE9 GetInstanceTexturePath(const char* path, const char* filename, D3DXIMAGE_INFO* imageInfo = nullptr);
	
	LPDIRECT3DTEXTURE9 GetInstanceTexturePathSolo(const char* path, const char* filename,D3DXIMAGE_INFO* imageInfo = nullptr);

	LPDIRECT3DTEXTURE9 GetInstanceTexture(const char* path, const char* fileName);

};

