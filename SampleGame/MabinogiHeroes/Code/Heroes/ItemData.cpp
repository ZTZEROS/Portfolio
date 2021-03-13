#include "stdafx.h"
#include "ItemData.h"


ItemData::ItemData()
{
}


ItemData::~ItemData()
{
}

void ItemData::Initialize()
{
	
	//테스트용
	if (ItemList.find("testPotion") == ItemList.end())
	{
		stItem anti_binding_potion("gold_potion_bottle.tga", Item_Kind::Item_Potion, 10, 10, 10, 1);
		ItemList.insert(make_pair("testPotion", anti_binding_potion));
	}

	if (ItemList.find("testSource") == ItemList.end())
	{
		stItem anti_binding_potion("mmatch_potion_balance.tga", Item_Kind::Item_Weapon, 10, 10, 10, 1);
		ItemList.insert(make_pair("testSource", anti_binding_potion));
	}

	//무기
	if (ItemList.find("IW_DualSword1") == ItemList.end())
	{
		stItem anti_binding_potion("IW_DualSword1.tga", Item_Kind::Item_Weapon, 10, 0, 10, 1);
		ItemList.insert(make_pair("IW_DualSword1", anti_binding_potion));
	}

	if (ItemList.find("IW_DualSword80") == ItemList.end())
	{
		stItem anti_binding_potion("IW_DualSword80.tga", Item_Kind::Item_Weapon, 80, 0, 80, 1);
		ItemList.insert(make_pair("IW_DualSword80", anti_binding_potion));
	}

	//나이트 호크 시리즈
	if (ItemList.find("IA_NightHawk") == ItemList.end())
	{
		stItem anti_binding_potion("IA_NightHawk.tga", Item_Kind::Item_HandArmor, 10, 10, 10, 1);
		ItemList.insert(make_pair("IA_NightHawk", anti_binding_potion));
	}
	if (ItemList.find("IF_NightHawk") == ItemList.end())
	{
		stItem anti_binding_potion("IF_NightHawk.tga", Item_Kind::Item_FootArmor, 10, 10, 10, 1);
		ItemList.insert(make_pair("IF_NightHawk", anti_binding_potion));
	}
	if (ItemList.find("IH_NightHawk") == ItemList.end())
	{
		stItem anti_binding_potion("IH_NightHawk.tga", Item_Kind::Item_HeadArmor, 10, 10, 10, 1);
		ItemList.insert(make_pair("IH_NightHawk", anti_binding_potion));
	}
	if (ItemList.find("IU_NightHawk") == ItemList.end())
	{
		stItem anti_binding_potion("IU_NightHawk.tga", Item_Kind::Item_UpperAromor, 10, 10, 10, 1);
		ItemList.insert(make_pair("IU_NightHawk", anti_binding_potion));
	}
	if (ItemList.find("IL_NightHawk") == ItemList.end())
	{
		stItem anti_binding_potion("IL_NightHawk.tga", Item_Kind::Item_LowerArmor, 10, 10, 10, 1);
		ItemList.insert(make_pair("IL_NightHawk", anti_binding_potion));
	}

	//아미겟돈 시리즈
	if (ItemList.find("IA_Armageddon") == ItemList.end())
	{
		stItem anti_binding_potion("IA_Armageddon.tga", Item_Kind::Item_HandArmor, 1, 12, 10, 1);
		ItemList.insert(make_pair("IA_Armageddon", anti_binding_potion));
	}
	if (ItemList.find("IF_Armageddon") == ItemList.end())
	{
		stItem anti_binding_potion("IF_Armageddon.tga", Item_Kind::Item_FootArmor, 1, 12, 10, 1);
		ItemList.insert(make_pair("IF_Armageddon", anti_binding_potion));
	}
	if (ItemList.find("IH_Armageddon") == ItemList.end())
	{
		stItem anti_binding_potion("IH_Armageddon.tga", Item_Kind::Item_HeadArmor, 1, 15, 10, 1);
		ItemList.insert(make_pair("IH_Armageddon", anti_binding_potion));
	}
	if (ItemList.find("IU_Armageddon") == ItemList.end())
	{
		stItem anti_binding_potion("IU_Armageddon.tga", Item_Kind::Item_UpperAromor, 1, 20, 10, 1);
		ItemList.insert(make_pair("IU_Armageddon", anti_binding_potion));
	}
	if (ItemList.find("IL_Armageddon") == ItemList.end())
	{
		stItem anti_binding_potion("IL_Armageddon.tga", Item_Kind::Item_LowerArmor, 1, 20, 10, 1);
		ItemList.insert(make_pair("IL_Armageddon", anti_binding_potion));
	}

	//널
	if (ItemList.find("Null") == ItemList.end())
	{
		stItem Null("Null.dds", Item_Kind::Item_End, 0, 0, 0, 0);
		ItemList.insert(make_pair("Null", Null));
	}
}

void ItemData::Destroy()
{
	ItemList.clear();
}

stItem ItemData::findInstanceItem(char * name)
{
	if (ItemList.find(name)->first.c_str())
	{
		return ItemList.find(name)->second;
	}

	return ItemList.find("Null")->second;
}

stItem ItemData::MakeInstanceItem(char * name)
{
	stItem temp;
	temp = findInstanceItem(name);
	stItem returnTemp(name, temp.kind, temp.atk, temp.def, temp.gold, temp.level);
	stItem sword("anti_binding_potion.tga", Item_Kind::Item_Weapon, 10, 10, 10, 1);
	return sword;
}
