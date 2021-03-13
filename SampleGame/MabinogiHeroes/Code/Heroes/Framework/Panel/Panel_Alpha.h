#pragma once
class Panel_Alpha
{
public:
	Panel_Alpha();
	~Panel_Alpha();

	virtual void Initialize(float xSize, float ySize, D3DCOLOR color);
	virtual void Destroy();
	virtual void Update();
	virtual void Render();

	inline const D3DXVECTOR3& GetPosition(){ return position; }
	inline void SetPosition(const D3DXVECTOR3& pos){ position = pos; }
	inline void SetRotationY(float angle){ rotationY = angle; }

protected:
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	std::vector<FVF_PositionColorTexture> vertex;

	D3DXMATRIXA16 world;
	float rotationY = 0.0f;
	D3DMATERIAL9 material;
	LPDIRECT3DTEXTURE9 texture = nullptr;
};

