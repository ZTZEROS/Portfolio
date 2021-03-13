#include "stdafx.h"
#include "ObjLoader.h"

const char* ObjLoader::OBJ_DIRECTORY = "../ResourceData/";

ObjLoader::ObjLoader()
{
}


ObjLoader::~ObjLoader()
{
}

void ObjLoader::LoadObjFileData(std::vector<ObjFileGroup*>& group, char* fileName)
{
	std::map<std::string, MaterialTexture*> materialTextures;

	std::vector<D3DXVECTOR3> pos;
	std::vector<D3DXVECTOR2> uv;
	std::vector<D3DXVECTOR3> normal;
	std::vector<FVF_PositionNormalTexture> vertex;
	std::string materialName;

	char fullPath[512];
	strcpy_s(fullPath, OBJ_DIRECTORY);
	strcat_s(fullPath, fileName);
	FILE* fp;
	fopen_s(&fp, fullPath, "r");

	if ( fp != nullptr )
	{
		group.clear();
		while ( !feof(fp) )
		{
			char tempBuffer[1024];
			fgets(tempBuffer, 1024, fp);
			//OutputDebugString(tempBuffer);
			
			if ( tempBuffer[0] == 'm' )
			{
				char mtlLib[1024];
				sscanf_s(tempBuffer, "%*s %s", mtlLib, 1024);

				LoadMtlFileData(materialTextures, mtlLib);
			}
			else if ( tempBuffer[0] == 'g' || tempBuffer[0] == 'o' )
			{
				if ( !vertex.empty() )
				{
					MaterialTexture* mtl = nullptr;
					if ( materialTextures.find(materialName) != materialTextures.end() )
					{
						mtl = materialTextures[materialName];
					}

					ObjFileGroup* newGroup = new ObjFileGroup(mtl, vertex);
					vertex.clear();
					group.push_back(newGroup);
				}
			}
			else if ( tempBuffer[0] == 'v' )
			{
				if ( tempBuffer[1] == 't' )
				{
					float u, v;
					sscanf_s(tempBuffer, "%*s %f %f %*f", &u, &v);
					//uv.push_back(D3DXVECTOR2(u, v));
					uv.push_back(D3DXVECTOR2(u, 1-v));		//블렌더로 뽑은거라 오른손 법칙
				}
				else if ( tempBuffer[1] == 'n' )
				{
					float x, y, z;
					sscanf_s(tempBuffer, "%*s %f %f %f", &x, &y, &z);
					//normal.push_back(D3DXVECTOR3(x, y, z));
					normal.push_back(D3DXVECTOR3(x, z, y));	//블렌더로 뽑은거라 오른손 법칙
				}
				else
				{
					float x, y, z;
					sscanf_s(tempBuffer, "%*s %f %f %f", &x, &y, &z);
					//pos.push_back(D3DXVECTOR3(x, y, z));
					pos.push_back(D3DXVECTOR3(x, z, y));	//블렌더로 뽑은거라 오른손 법칙
				}
			}
			else if ( tempBuffer[0] == 'u' )
			{
				char mtlName[1024];
				sscanf_s(tempBuffer, "%*s %s", mtlName, 1024);
				materialName = mtlName;
			}
			else if ( tempBuffer[0] == 'f' )
			{
				FVF_PositionNormalTexture v[4];

				char *p = tempBuffer + 2;	// 앞쪽 "f " 제거
				unsigned int i = 0;

				//for ( int i = 0; i < 3; ++i )	//무조건 삼각형으로 이루어졌다고 가정
				while(i < 4)
				{
					while ( *p == ' ' || *p == '\t' )	//스페이스나 탭 무시
						p++;

					if ( *p == '\0' || *p == '\r' || *p == '\n' )	//줄이 끝나면 종료
						break;

					
					int index[3];
					index[0] = strtoul(p, &p, 10);	//첫숫자 잘라내기 (원본, 잘리고 남은 스트링, 진수)
					if ( *p && *p == '/' )
					{
						p++;	// '/'면 다음으로
						index[1] = strtoul(p, &p, 10);
						if ( *p && *p == '/' )
						{
							p++;
							index[2] = strtoul(p, &p, 10);
						}
					}

					if ( index[0] > 0 ) v[i].pos = pos[index[0] - 1];
					else v[i].pos = D3DXVECTOR3(0, 0, 0);
					
					if ( index[1] > 0 ) v[i].tex = uv[index[1] - 1];
					else v[i].tex = D3DXVECTOR2(0, 0);
					
					if ( index[2] > 0 ) v[i].normal = normal[index[2] - 1];
					else v[i].normal = D3DXVECTOR3(0, 1, 0);

					++i;
				}
				
				vertex.push_back(v[0]);	//블렌더로 뽑은거라 오른손 법칙
				vertex.push_back(v[1]);
				vertex.push_back(v[2]);

				if (i > 3) vertex.push_back(v[3]);
			}
		}

		if ( !vertex.empty() )
		{
			MaterialTexture* mtl = nullptr;
			if ( materialTextures.find(materialName) != materialTextures.end() )
			{
				mtl = materialTextures[materialName];
			}

			ObjFileGroup* newGroup = new ObjFileGroup(mtl, vertex);
			vertex.clear();
			group.push_back(newGroup);
		}

		fclose(fp);
	}

	for (auto iter = materialTextures.begin(); iter != materialTextures.end(); ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
}

void ObjLoader::LoadMtlFileData(std::map<std::string, MaterialTexture*>& materialTextures, char* fileName)
{
	char fullPath[512];
	strcpy_s(fullPath, OBJ_DIRECTORY);
	strcat_s(fullPath, fileName);

	FILE* fp;
	fopen_s(&fp, fullPath, "r");

	if (fp != nullptr)
	{
		std::string materialName = "";
		MaterialTexture* newMT = nullptr;

		while (!feof(fp))
		{
			char tempBuffer[1024];
			fgets(tempBuffer, 1024, fp);
			//OutputDebugString(tempBuffer);
			if (tempBuffer[0] == '#')
			{
				continue;
			}
			else if (tempBuffer[0] == 'n')
			{
				if (newMT)
				{
					materialTextures[materialName] = newMT;
					newMT = nullptr;
				}

				char mtlName[1024];
				sscanf_s(tempBuffer, "%*s %s", mtlName, 1024);
				materialName = mtlName;
				if (materialTextures.find(materialName) == materialTextures.end())
				{
					newMT = new MaterialTexture;
				}
				else
				{
					newMT = materialTextures[materialName];
				}
			}
			else if (tempBuffer[0] == 'K')
			{
				if (tempBuffer[1] == 'a')
				{
					float r, g, b;
					sscanf_s(tempBuffer, "%*s %f %f %f", &r, &g, &b);
					if (newMT)
					{
						newMT->material.Ambient.r = r;
						newMT->material.Ambient.g = g;
						newMT->material.Ambient.b = b;
						newMT->material.Ambient.a = 1.0f;
					}
				}
				else if (tempBuffer[1] == 'd')
				{
					float r, g, b;
					sscanf_s(tempBuffer, "%*s %f %f %f", &r, &g, &b);
					if (newMT)
					{
						newMT->material.Diffuse.r = r;
						newMT->material.Diffuse.g = g;
						newMT->material.Diffuse.b = b;
						newMT->material.Diffuse.a = 1.0f;
					}
				}
				else if (tempBuffer[1] == 's')
				{
					float r, g, b;
					sscanf_s(tempBuffer, "%*s %f %f %f", &r, &g, &b);
					if (newMT)
					{
						newMT->material.Specular.r = r;
						newMT->material.Specular.g = g;
						newMT->material.Specular.b = b;
						newMT->material.Specular.a = 1.0f;
					}
				}
			}
			else if (tempBuffer[0] == 'm' && tempBuffer[5] == 'd')
			{
				char szTexFile[1024];
				sscanf_s(tempBuffer, "%*s %s", szTexFile, 1024);
				if (newMT)
				{
					newMT->texture = TextureManager::GetTexture(szTexFile);
				}
			}
		}

		if (newMT)
		{
			materialTextures[materialName] = newMT;
			newMT = nullptr;
		}

		fclose(fp);
	}

}

void ObjLoader::LoadObj_OnlyVertex(OUT std::vector<D3DXVECTOR3>& vertex, char* fileName)
{
	char fullPath[512];
	strcpy_s(fullPath, OBJ_DIRECTORY);
	strcat_s(fullPath, fileName);

	FILE* fp = nullptr;
	fopen_s(&fp, fullPath, "r");

	if (fp != nullptr)
	{
		vertex.clear();
		std::vector<D3DXVECTOR3> point;

		while (!feof(fp))
		{
			char tempBuffer[1024];
			fgets(tempBuffer, 1024, fp);
			if (tempBuffer[0] == 'v')
			{
				if (tempBuffer[1] == ' ')
				{
					float x, y, z;
					sscanf_s(tempBuffer, "%*s %f %f %f", &x, &z, &y);
					point.push_back(D3DXVECTOR3(x, y, z));
				}
			}
			else if (tempBuffer[0] == 'f')
			{
				int index[3];
				sscanf_s(tempBuffer, "%*s %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &index[0], &index[2], &index[1]);
				for (int i = 0; i < 3; ++i)
				{
					vertex.push_back(point[index[i] - 1]);
				}
			}
		}

		fclose(fp);
	}
}