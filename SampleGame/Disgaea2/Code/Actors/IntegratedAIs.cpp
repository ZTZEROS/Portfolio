#include "stdafx.h"
#include "IntegratedAIs.h"

IntegratedAIs::IntegratedAIs() {}
IntegratedAIs::~IntegratedAIs() {}

void IntegratedAIs::Initialize()
{
	ZeroMemory(&Sum, sizeof(Sum));
	CenterOfEnemies = PointMake(0, 0);

	//Sum.OfEnemies.MaxHP = Sum.OfEnemies.MaxSP = 0;
	//Sum.OfEnemies.CurrentHP = Sum.OfEnemies.CurrentSP = 0;
	//Sum.OfEnemies.Attack = Sum.OfEnemies.Defence = 0;
	//Sum.OfEnemies.Hit = Sum.OfEnemies.Speed = 0;
	//Sum.OfEnemies.Magic = Sum.OfEnemies.Resistance = 0;

	//Sum.OfHeroes.MaxHP = Sum.OfHeroes.MaxSP = 0;
	//Sum.OfHeroes.CurrentHP = Sum.OfHeroes.CurrentSP = 0;
	//Sum.OfHeroes.Attack = Sum.OfHeroes.Defence = 0;
	//Sum.OfHeroes.Hit = Sum.OfHeroes.Speed = 0;
	//Sum.OfHeroes.Magic = Sum.OfHeroes.Resistance = 0;

	memset(&Average, 0, sizeof(Average));
	CenterOfHeroes = PointMake(0, 0);

	//Average.OfEnemies.MaxHP = Average.OfEnemies.MaxSP = 0;
	//Average.OfEnemies.CurrentHP = Average.OfEnemies.CurrentSP = 0;
	//Average.OfEnemies.Attack = Average.OfEnemies.Defence = 0;
	//Average.OfEnemies.Hit = Average.OfEnemies.Speed = 0;
	//Average.OfEnemies.Magic = Average.OfEnemies.Resistance = 0;

	//Average.OfHeroes.MaxHP = Average.OfHeroes.MaxSP = 0;
	//Average.OfHeroes.CurrentHP = Average.OfHeroes.CurrentSP = 0;
	//Average.OfHeroes.Attack = Average.OfHeroes.Defence = 0;
	//Average.OfHeroes.Hit = Average.OfHeroes.Speed = 0;
	//Average.OfHeroes.Magic = Average.OfHeroes.Resistance = 0;
}

void IntegratedAIs::Release()
{
}

void IntegratedAIs::Update()
{
}

void IntegratedAIs::Render()
{
}

void IntegratedAIs::FundamentalAI()
{
	for (EM->SetEnemiesVI((*EM->GetEnemiesVAddress()).begin()); (*EM->GetEnemiesVIAddress()) != (*EM->GetEnemiesVAddress()).end(); EM->SetEnemiesVI(++(*EM->GetEnemiesVIAddress())))
	{
		Sum.OfEnemies.MaxHP += (**EM->GetEnemiesVIAddress())->GetMaxHP();
		Sum.OfEnemies.MaxSP += (**EM->GetEnemiesVIAddress())->GetMaxSP();
		Sum.OfEnemies.CurrentHP += (**EM->GetEnemiesVIAddress())->GetCurrentHP();
		Sum.OfEnemies.CurrentSP += (**EM->GetEnemiesVIAddress())->GetCurrentSP();
		Sum.OfEnemies.Attack += (**EM->GetEnemiesVIAddress())->GetAttack();
		Sum.OfEnemies.Defence += (**EM->GetEnemiesVIAddress())->GetDefence();
		Sum.OfEnemies.Hit += (**EM->GetEnemiesVIAddress())->GetHit();
		Sum.OfEnemies.Speed += (**EM->GetEnemiesVIAddress())->GetSpeed();
		Sum.OfEnemies.Magic += (**EM->GetEnemiesVIAddress())->GetMagic();
		Sum.OfEnemies.Resistance += (**EM->GetEnemiesVIAddress())->GetResistance();

		Sum.OfEnemies.Position.x += (**EM->GetEnemiesVIAddress())->GetTileX();
		Sum.OfEnemies.Position.y += (**EM->GetEnemiesVIAddress())->GetTileY();
	}

	for (int i = 0; i < EM->GetHMAddress()->GetHeroesVAddress()->size(); ++i)
	{
		Sum.OfHeroes.MaxHP += (*EM->GetHMAddress()->GetHeroesVAddress())[i]->GetMaxHP();
		Sum.OfHeroes.MaxSP += (*EM->GetHMAddress()->GetHeroesVAddress())[i]->GetMaxSP();
		Sum.OfHeroes.CurrentHP += (*EM->GetHMAddress()->GetHeroesVAddress())[i]->GetCurrentHP();
		Sum.OfHeroes.CurrentSP += (*EM->GetHMAddress()->GetHeroesVAddress())[i]->GetCurrentSP();
		Sum.OfHeroes.Attack += (*EM->GetHMAddress()->GetHeroesVAddress())[i]->GetAttack();
		Sum.OfHeroes.Defence += (*EM->GetHMAddress()->GetHeroesVAddress())[i]->GetDefence();
		Sum.OfHeroes.Hit += (*EM->GetHMAddress()->GetHeroesVAddress())[i]->GetHit();
		Sum.OfHeroes.Speed += (*EM->GetHMAddress()->GetHeroesVAddress())[i]->GetSpeed();
		Sum.OfHeroes.Magic += (*EM->GetHMAddress()->GetHeroesVAddress())[i]->GetMagic();
		Sum.OfHeroes.Resistance += (*EM->GetHMAddress()->GetHeroesVAddress())[i]->GetResistance();

		Sum.OfHeroes.Position.x += (*EM->GetHMAddress()->GetHeroesVAddress())[i]->GetTileX();
		Sum.OfHeroes.Position.y += (*EM->GetHMAddress()->GetHeroesVAddress())[i]->GetTileY();
	}

	Average.OfEnemies.MaxHP = Sum.OfEnemies.MaxHP / (*EM->GetEnemiesVAddress()).size();
	Average.OfEnemies.MaxSP = Sum.OfEnemies.MaxSP / (*EM->GetEnemiesVAddress()).size();
	Average.OfEnemies.CurrentHP = Sum.OfEnemies.CurrentHP / (*EM->GetEnemiesVAddress()).size();
	Average.OfEnemies.CurrentSP = Sum.OfEnemies.CurrentSP / (*EM->GetEnemiesVAddress()).size();
	Average.OfEnemies.Attack = Sum.OfEnemies.Attack / (*EM->GetEnemiesVAddress()).size();
	Average.OfEnemies.Defence = Sum.OfEnemies.Defence / (*EM->GetEnemiesVAddress()).size();
	Average.OfEnemies.Hit = Sum.OfEnemies.Hit / (*EM->GetEnemiesVAddress()).size();
	Average.OfEnemies.Speed = Sum.OfEnemies.Speed / (*EM->GetEnemiesVAddress()).size();
	Average.OfEnemies.Magic = Sum.OfEnemies.Magic / (*EM->GetEnemiesVAddress()).size();
	Average.OfEnemies.Resistance = Sum.OfEnemies.Resistance / (*EM->GetEnemiesVAddress()).size();

	Average.OfEnemies.Position.x = Sum.OfEnemies.Position.x /= (*EM->GetEnemiesVAddress()).size();
	Average.OfEnemies.Position.y = Sum.OfEnemies.Position.y /= (*EM->GetEnemiesVAddress()).size();


	Average.OfHeroes.MaxHP = Sum.OfHeroes.MaxHP / (EM->GetHMAddress()->GetHeroesVAddress())->size();
	Average.OfHeroes.MaxSP = Sum.OfHeroes.MaxSP / (EM->GetHMAddress()->GetHeroesVAddress())->size();
	Average.OfHeroes.CurrentHP = Sum.OfHeroes.CurrentHP / (EM->GetHMAddress()->GetHeroesVAddress())->size();
	Average.OfHeroes.CurrentSP = Sum.OfHeroes.CurrentSP / (EM->GetHMAddress()->GetHeroesVAddress())->size();
	Average.OfHeroes.Attack = Sum.OfHeroes.Attack / (EM->GetHMAddress()->GetHeroesVAddress())->size();
	Average.OfHeroes.Defence = Sum.OfHeroes.Defence / (EM->GetHMAddress()->GetHeroesVAddress())->size();
	Average.OfHeroes.Hit = Sum.OfHeroes.Hit / (EM->GetHMAddress()->GetHeroesVAddress())->size();
	Average.OfHeroes.Speed = Sum.OfHeroes.Speed / (EM->GetHMAddress()->GetHeroesVAddress())->size();
	Average.OfHeroes.Magic = Sum.OfHeroes.Magic / (EM->GetHMAddress()->GetHeroesVAddress())->size();
	Average.OfHeroes.Resistance = Sum.OfHeroes.Resistance / (EM->GetHMAddress()->GetHeroesVAddress())->size();

	Average.OfHeroes.Position.x = Sum.OfHeroes.Position.x /= (EM->GetHMAddress()->GetHeroesVAddress())->size();
	Average.OfHeroes.Position.y = Sum.OfHeroes.Position.y /= (EM->GetHMAddress()->GetHeroesVAddress())->size();

	if (Sum.OfEnemies.CurrentHP > Sum.OfHeroes.CurrentHP) Advantage = 1;
	else Advantage = 0;

	SelectDestination();
}

void IntegratedAIs::GeneticAI()
{

}

void IntegratedAIs::SelectDestination()
{
	CenterOfEnemies = PointMake(Average.OfEnemies.Position.x, Average.OfEnemies.Position.y);
	CenterOfHeroes = PointMake(Average.OfHeroes.Position.x, Average.OfHeroes.Position.y);

	if (Advantage)
	{
		if (Average.OfEnemies.Position.x > Average.OfHeroes.Position.x) CenterOfEnemies = (PointMake(Average.OfEnemies.Position.x - 1, Average.OfEnemies.Position.y));
		if (Average.OfEnemies.Position.x < Average.OfHeroes.Position.x) CenterOfEnemies = (PointMake(Average.OfEnemies.Position.x + 1, Average.OfEnemies.Position.y));
		if (Average.OfEnemies.Position.y > Average.OfHeroes.Position.y) CenterOfEnemies = (PointMake(Average.OfEnemies.Position.x, Average.OfEnemies.Position.y - 1));
		if (Average.OfEnemies.Position.y < Average.OfHeroes.Position.y) CenterOfEnemies = (PointMake(Average.OfEnemies.Position.x, Average.OfEnemies.Position.y + 1));
	}
	else
	{
		if (Average.OfEnemies.Position.x > Average.OfHeroes.Position.x) CenterOfEnemies = (PointMake(Average.OfEnemies.Position.x + 1, Average.OfEnemies.Position.y));
		if (Average.OfEnemies.Position.x < Average.OfHeroes.Position.x) CenterOfEnemies = (PointMake(Average.OfEnemies.Position.x - 1, Average.OfEnemies.Position.y));
		if (Average.OfEnemies.Position.y > Average.OfHeroes.Position.y) CenterOfEnemies = (PointMake(Average.OfEnemies.Position.x, Average.OfEnemies.Position.y + 1));
		if (Average.OfEnemies.Position.y < Average.OfHeroes.Position.y) CenterOfEnemies = (PointMake(Average.OfEnemies.Position.x, Average.OfEnemies.Position.y - 1));
	}
}

void IntegratedFundamentalAI() //최외곽선 안으로 들어가지 않고 아군부대 중심위치를 기준으로 대형을 갖추어 가장 가까운 적을 공격한다.
{

}

void IntegratedGeneticAI() //랜덤한 순서로 랜덤한 행위를 하여 상대측 체력을 가장 많이 줄인 일련의 행위들의 패턴을 택하여 실행한다.
{
	int Step = 0;

	if (Step == 0)
	{
		RND;
		RND;
	}
}

void SelectDestination()
{

}