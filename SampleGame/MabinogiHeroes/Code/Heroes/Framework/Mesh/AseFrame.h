#pragma once
#include <vector>
#include "../FVF.h"
#include "MaterialTexture.h"

struct PositionSample
{
	int frame;
	D3DXVECTOR3 pos;
};

struct RotateSample
{
	int frame;
	D3DXQUATERNION quaternion;
};

class AseFrame
{
public:
	AseFrame();
	~AseFrame();

	void Update(int keyFrame, D3DXMATRIXA16* parent);
	void Render();	
	void Destroy();

	void AddChild(AseFrame* child);
		
	void CalcOrigLocalTransform(D3DXMATRIXA16* parent);

	// Get/Set ÇÔ¼öµé
	void GetLocalTranslate(IN int keyFrame, OUT D3DXMATRIXA16& matrix);
	void GetLocalRotate(IN int keyFrame, OUT D3DXMATRIXA16& matrix);

	inline D3DXMATRIXA16& GetWorldTransform(){ return world; }
	inline std::vector<FVF_PositionNormalTexture>& GetVertexArray(){ return vertexArray; }
	inline std::vector<PositionSample>& GetPosTrack(){ return posTrack; }
	inline std::vector<RotateSample>& GetRotTrack(){ return rotTrack; }

	void SetWorldTransform(D3DXMATRIXA16* matrix);
	void SetTextureOn(bool on);

	inline void SetMaterialTexture(MaterialTexture* mt){ mt->AddReference(); materialTexture = mt; }
	inline void SetNodeName(std::string name){ nodeName = name; }	

	
private:
	std::vector<AseFrame*> childArray;

	D3DXMATRIXA16 world;
	D3DXMATRIXA16 local;

	MaterialTexture* materialTexture = nullptr;
	
	std::string nodeName;	
	std::vector<FVF_PositionNormalTexture> vertexArray;	
	std::vector<PositionSample> posTrack;
	std::vector<RotateSample> rotTrack;	

	bool textureOn = true;
};


