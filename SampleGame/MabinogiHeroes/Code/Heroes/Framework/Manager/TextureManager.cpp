#include "stdafx.h"
#include "TextureManager.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

LPDIRECT3DTEXTURE9 TextureManager::GetInstanceTexture(const char* filename, D3DXIMAGE_INFO* imageInfo /*= nullptr*/)
{
	if (textureDataMap.find(filename) == textureDataMap.end())
	{
		char fullPath[256];
		strcpy_s(fullPath, TEXTURE_DIRECTORY);
		strcat_s(fullPath, filename);
		D3DXCreateTextureFromFileExA(
			GameManager::GetDevice(),
			fullPath,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0,
			&textureDataMap[filename].second,
			nullptr,
			&textureDataMap[filename].first);
	}
	if (imageInfo)
	{
		*imageInfo = textureDataMap[filename].second;
	}
	return textureDataMap[filename].first;
}

LPDIRECT3DTEXTURE9 TextureManager::GetInstanceTexturePath(const char * path, const char * filename, D3DXIMAGE_INFO * imageInfo)
{
	if (textureDataMap.find(filename) == textureDataMap.end())
	{
		char fullPath[256];
		strcpy_s(fullPath, path);
		strcat_s(fullPath, filename);

		D3DXCreateTextureFromFileExA(
			GameManager::GetDevice(),
			fullPath,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0,
			&textureDataMap[filename].second,
			nullptr,
			&textureDataMap[filename].first);
	}
	if (imageInfo)
	{
		*imageInfo = textureDataMap[filename].second;
	}
	return textureDataMap[filename].first;
}

LPDIRECT3DTEXTURE9 TextureManager::GetInstanceTexturePathSolo(const char* path, const char* filename, D3DXIMAGE_INFO* imageInfo)
{
	
		char fullPath[256];
		strcpy_s(fullPath, path);
		strcat_s(fullPath, filename);

		D3DXCreateTextureFromFileExA(
			GameManager::GetDevice(),
			fullPath,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0,
			&tempInfo,
			nullptr,
			&temp);
	if (imageInfo)
	{
		*imageInfo = tempInfo;
	}
	return temp;
}
void TextureManager::Destroy()
{
	int k = 0;
	for (auto iter = textureDataMap.begin(); iter != textureDataMap.end(); ++iter)
	{
		k++;
		SAFE_RELEASE(iter->second.first);
	}

	/*if (temp != nullptr)
	{
		SAFE_RELEASE(temp);
	}*/
	textureDataMap.clear();

}


LPDIRECT3DTEXTURE9 TextureManager::GetInstanceTexture(const char* path, const char* fileName)
{
	if (textureDataMap.find(fileName) == textureDataMap.end())
	{
		char fullPath[512];
		strcpy_s(fullPath, path);
		strcat_s(fullPath, fileName);

		
		D3DXCreateTextureFromFileExA(
			GameManager::GetDevice(),
			fullPath,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0,
			&textureDataMap[fileName].second,
			nullptr,
			&textureDataMap[fileName].first);
	}
	return textureDataMap[fileName].first;
}