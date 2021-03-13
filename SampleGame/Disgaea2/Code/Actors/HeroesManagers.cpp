#include "stdafx.h"
#include "HeroesManagers.h"

HeroesManagers::HeroesManagers() {}
HeroesManagers::~HeroesManagers() {}

HRESULT HeroesManagers::Initialize()
{
	//Flonne.Initialize();
	//Etna.Initialize();
	//Prier.Initialize("", 0, 0);

	//HeroesV.push_back(FLONNE);
	//HeroesV.push_back(ETNA);
	//HeroesV.push_back(Prier);

	//Heroes* Flonne;
	//Flonne = new Flonnes;
	//Flonne->Initialize("Flonne", 1, 0);
	//HeroesV.push_back(Flonne);

	//Heroes* Etna;
	//Etna = new Etnas;
	//Etna->Initialize("Etna", 1, 1);
	//HeroesV.push_back(Etna);

	Heroes* Prier;
	Prier = new Priers;
	Prier->Initialize("Prier", 15, 14);
	HeroesV.push_back(Prier);

	//Heroes* Adell;
	//Adell = new Adells;
	//Adell->Initialize("Adell", 16, 16);
	//HeroesV.push_back(Adell);

	Heroes* Fenrich;
	Fenrich = new Fenrichs;
	Fenrich->Initialize("Fenrich", 14, 15);
	HeroesV.push_back(Fenrich);
	
	Heroes* Axel;
	Axel = new Axels;
	Axel->Initialize("Axel", 15, 16);
	HeroesV.push_back(Axel);

	Act = 0;

	return S_OK;
}

void HeroesManagers::Release()
{
	//Flonne.Release();
	//Etna.Release();

	for (HeroesVI = HeroesV.begin(); HeroesVI != HeroesV.end(); ++HeroesVI)
	{
		(*HeroesVI)->Release();
	}
}

void HeroesManagers::Update()
{
	//Flonne.Update();
	//Etna.Update();

	for (HeroesVI = HeroesV.begin(); HeroesVI != HeroesV.end(); ++HeroesVI)
	{
		(*HeroesVI)->Update();

		//if ((*HeroesVI)->GetAttacked())
		//{
		//	for (int i = 0; i < EM->GetEnemiesV().size(); ++i)
		//	{
		//		if ((*HeroesVI)->GetTargetTile().x == EM->GetEnemiesVTileX(i) && (*HeroesVI)->GetTargetTile().y == EM->GetEnemiesVTileY(i))
		//		{
		//			EM->GetEnemiesVAttacked(i);
		//			EM->SetEnemiesVCurrentHP(i, EM->GetEnemiesVCurrentHP(i) - 10);
		//			(*HeroesVI)->SetAttacked(0);
		//			break;
		//		}
		//	}
		//}
	}
	
	if (Act)
	{
		//(*HeroesActionEntryV[0])->Attack((*HeroesActionEntryV[0])->GetTargetTile());
		//(*HeroesActionEntryV[0])->Action();
		//
		//for (int i = 0; i < EM->GetEnemiesV().size(); ++i)
		//{
		//	if (((*HeroesActionEntryV[0])->GetTargetTile().x == EM->GetEnemiesV()[i]->GetTileX()) && ((*HeroesActionEntryVI[0])->GetTargetTile().y == EM->GetEnemiesV()[i]->GetTileY()))
		//		EM->RunEnemiesVIsAttacked(i, PointMake((*HeroesActionEntryV[0])->GetTileX(), (*HeroesActionEntryV[0])->GetTileY()));
		//}

		for (HeroesActionEntryVI = HeroesActionEntryV.begin(); HeroesActionEntryVI != HeroesActionEntryV.end(); ++HeroesActionEntryVI)
		{
			(**HeroesActionEntryVI)->Attack((**HeroesActionEntryVI)->GetTargetTile());
			(**HeroesActionEntryVI)->Action();
			//한 번 실행 후 벡터에서 삭제, Act = 0;, 공격이 끝나면 == 데미지를 계산해서 출력한 후이면 Act = 1;, 다시 실행
		
		
		
			for (int i = 0; i < EM->GetEnemiesV().size(); ++i)
			{
				if (((**HeroesActionEntryVI)->GetTargetTile().x == EM->GetEnemiesV()[i]->GetTileX()) && ((**HeroesActionEntryVI)->GetTargetTile().y == EM->GetEnemiesV()[i]->GetTileY()))
					EM->RunEnemiesVIsAttacked(i, PointMake((**HeroesActionEntryVI)->GetTileX(), (**HeroesActionEntryVI)->GetTileY()));
			}
		}

		Act = 0;

		HeroesActionEntryV.erase(HeroesActionEntryV.begin());
	}

	
}

void HeroesManagers::Render()
{
	//Flonne.Render();
	//Etna.Render();

	for (HeroesVI = HeroesV.begin(); HeroesVI != HeroesV.end(); ++HeroesVI)
	{
		(*HeroesVI)->Render();
	}
}
