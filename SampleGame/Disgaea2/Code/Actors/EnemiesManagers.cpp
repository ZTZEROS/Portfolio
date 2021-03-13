#include "stdafx.h"
#include "EnemiesManagers.h"

EnemiesManagers::EnemiesManagers() {}
EnemiesManagers::~EnemiesManagers() {}

HRESULT EnemiesManagers::Initialize()
{
	Enemies* ArmorKnightFemale1;
	ArmorKnightFemale1 = new ArmorKnightFemales;
	ArmorKnightFemale1->Initialize("ArmorKnightFemale", 15, 12);
	EnemiesV.push_back(ArmorKnightFemale1);
	
	Enemies* ArmorKnightFemale2;
	ArmorKnightFemale2 = new ArmorKnightFemales;
	ArmorKnightFemale2->Initialize("ArmorKnightFemale", 15, 11);
	EnemiesV.push_back(ArmorKnightFemale2);

	Enemies* HealerMale1;
	HealerMale1 = new HealerMales;
	HealerMale1->Initialize("HealerMale", 16, 12);
	EnemiesV.push_back(HealerMale1);

	Enemies* HealerMale2;
	HealerMale2 = new HealerMales;
	HealerMale2->Initialize("HealerMale", 16, 11);
	EnemiesV.push_back(HealerMale2);

	Act = 0;

	return S_OK;
}

void EnemiesManagers::Release()
{
	for (EnemiesVI = EnemiesV.begin(); EnemiesVI != EnemiesV.end(); ++EnemiesVI)
	{
		(*EnemiesVI)->Release();
	}
}

void EnemiesManagers::Update()
{
	//IAI->FundamentalAI();
	//IAI->GeneticAI();
	
	Act = 1;

	for (EnemiesVI = EnemiesV.begin(); EnemiesVI != EnemiesV.end(); ++EnemiesVI)
	{
		(*EnemiesVI)->Update();

		if (!(*EnemiesVI)->GetMoved()) Act = 0;
	}
	
	if (Act)
	{
		//(**EnemiesActionEntryV.begin())->Attack(PointMake(HM->GetHeroesVTileX(0), HM->GetHeroesVTileX(0)));
		//(**EnemiesActionEntryV.begin())->Action();
		//EnemiesActionEntryV.erase(EnemiesActionEntryV.begin());

		for (EnemiesActionEntryVI = EnemiesActionEntryV.begin(); EnemiesActionEntryVI != EnemiesActionEntryV.end(); ++EnemiesActionEntryVI)
		{
			//(**EnemiesActionEntryVI)->GetTargetTile();
			(**EnemiesActionEntryVI)->Attack(PointMake(HM->GetHeroesVTileX(0), HM->GetHeroesVTileX(0)));
			(**EnemiesActionEntryVI)->Action();

			for (int i = 0; i < HM->GetHeroesV().size(); i++)
			{
				if (((**EnemiesActionEntryVI)->GetTargetTile().x == HM->GetHeroesV()[i]->GetTileX()) && ((**EnemiesActionEntryVI)->GetTargetTile().y == HM->GetHeroesV()[i]->GetTileX()))
					HM->RunHeroesVIsAttacked(i, PointMake((**EnemiesActionEntryVI)->GetTileX(), (**EnemiesActionEntryVI)->GetTileY()));
			}
		}

		Act = 0;

		EnemiesActionEntryV.erase(EnemiesActionEntryV.begin());
	}
}

void EnemiesManagers::Render()
{
	for (EnemiesVI = EnemiesV.begin(); EnemiesVI != EnemiesV.end(); ++EnemiesVI)
	{
		(*EnemiesVI)->Render();
	}
}
