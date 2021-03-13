#pragma once

enum Item_Kind
{
	Item_HeadArmor,
	Item_Weapon,
	Item_UpperAromor,
	Item_LowerArmor	,
	Item_HandArmor,
	Item_FootArmor,
	Item_Potion,
	Item_Source,
	Item_End
};

struct stItem
{
	char* name;
	int atk = 0;
	int def = 0;
	int gold = 0;
	int level = 1;
	bool equip = false;
	Item_Kind kind = Item_End;
	D3DXVECTOR2 size;
	D3DXIMAGE_INFO skinInfo;

	stItem() {}
	stItem(char* texName, Item_Kind _kind, int _atk, int _def, int _gold, int _level)
	{
	char* path = "../ResourceData/Texture/Item/Inven/";
	//ItemImage = TextureManager::GetTexturePathSolo(path, texName, nullptr);
	char* path2 = "../ResourceData/Texture/Item/Skin/";
	//SkinTexture = TextureManager::GetTexturePath(path2, texName, &skinInfo);
		name = texName;
		kind = _kind;
		atk = _atk;
		def = _def;
		gold = _gold;
		level = _level;
		size.x = (float)skinInfo.Width;
		size.y = (float)skinInfo.Height;
	}
	
};


