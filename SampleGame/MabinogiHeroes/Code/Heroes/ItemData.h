#pragma once
#include "itemBase.h"
class ItemData
{
private:
	ItemData();
	ItemData(ItemData& other);
	~ItemData();

public:
	
	static ItemData& Get()
	{
		static ItemData instance;
		return instance;
	}

	void Initialize();
	void Destroy();

	static inline stItem findItem(char* name) { return ItemData::Get().findInstanceItem(name); }
	static inline stItem MakeItem(char* name) { return ItemData::Get().findInstanceItem(name); };

private:
	std::map<std::string, stItem> ItemList;
	int preSize = 12;//예상 아이템 갯수

	stItem findInstanceItem(char* name);
	stItem MakeInstanceItem(char* name);

};

