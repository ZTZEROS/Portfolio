#pragma once
class ProgressBar
{
public:
	ProgressBar();
	~ProgressBar();

	void Initilize(char* front, char* behind);
	void Destory();
	void Render();
	void Update();

	void gaugeHeight(float currentGauge, float maxGauge);
	void gaugeWidth(float currentGauge, float maxGauge);

	inline void SetPostion(float x, float y, float z) { postion = D3DXVECTOR3(x, y, z);  D3DXMatrixTranslation(&Translation, postion.x, postion.y, postion.z); }
	inline RECT GetRect() { return RcProgress; }

private:
	D3DXVECTOR3 postion;
	D3DXMATRIX world;
	D3DXMATRIX Scale;
	D3DXMATRIX Rotation;
	D3DXMATRIX Translation;

	D3DXVECTOR2 size;

	RECT RcProgress;

	D3DXIMAGE_INFO FrontInfo;
	D3DXIMAGE_INFO BehindInfo;
	LPDIRECT3DTEXTURE9 FrontBar;
	LPDIRECT3DTEXTURE9 BehindBar;

	float width =0;
	float height =0;

};

