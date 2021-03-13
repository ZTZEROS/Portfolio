#pragma once

struct stPosition
{
	stPosition() {}
	stPosition(D3DXVECTOR3 _start, D3DXVECTOR3 _end, D3DXVECTOR3 _midle)
	{
		start = _start;
		end = _end;
		midle = _midle;
	}

	D3DXVECTOR3 start;
	D3DXVECTOR3 end;
	D3DXVECTOR3 midle;
	stPosition* child = nullptr;
};

class WindBlade
{
public:
	WindBlade();
	~WindBlade();

	void init();
	void release();
	void addVPosition(D3DXVECTOR3 _start, D3DXVECTOR3 _end);
	void render(D3DXMATRIXA16& world);

private:
	std::vector<FVF_PositionColor> vertices;
	D3DMATERIAL9 WindBladeMaterial;
};

