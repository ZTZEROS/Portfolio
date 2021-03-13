#pragma once
class MaterialTexture
{
public:
	
	MaterialTexture()
	{
		ZeroMemory(&material, sizeof(D3DMATERIAL9));
		refCount = 1;
	}

	~MaterialTexture()
	{
		assert(refCount == 0 && "���۷��� ī���Ͱ� 0�� �ƴϴ�!");		
	}

	void AddReference()
	{
		refCount++;
	}

	ULONG Release()
	{
		refCount--;
		if (refCount <= 0)
		{
			Destroy();
		}

		return refCount;
	}

	D3DMATERIAL9 material;
	LPDIRECT3DTEXTURE9 texture = nullptr;

private:
	UINT32 refCount = 1;

	void Destroy()
	{
		texture = nullptr;
		delete this;
	}
	
};

