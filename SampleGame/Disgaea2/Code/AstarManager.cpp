#include "stdafx.h"
#include "AstarManager.h"

AstarManager::AstarManager() {}
AstarManager::~AstarManager() {}

HRESULT AstarManager::init(void)
{
	Count = Start = 0;
	
	return S_OK;
}

void AstarManager::release(void) {}

void AstarManager::update(void)
{

}

void AstarManager::render(void)
{

}

//void AstarManager::SetTiles()
//{
//	StartTile = new tagTile;
//	//StartTile->X = ;
//	//StartTile->Y = ;
//	StartTile->Attribute = "Start";
//
//	EndTile = new tagTile;
//	//EndTile->X = ;
//	//EndTile->Y = ;
//	EndTile->Attribute = "End";
//
//	CurrentTile = StartTile;
//
//	for (int i = 0; i < TILENUMY; i++)
//	{
//		for (int j = 0; j < TILENUMX; j++)
//		{
//			if (j == StartTile->X && i == StartTile->Y)
//			{
//				TotalMapTileV.push_back(StartTile);
//				continue;
//			}
//			
//			if (j == EndTile->X && i == EndTile->Y)
//			{
//				TotalMapTileV.push_back(EndTile);
//				continue;
//			}
//
//			tagTile* tile = new tagTile;
//			TotalMapTileV.push_back(tile);
//		}
//	}
//}
//
//vector<tagTile*> AstarManager::AddOpenedTile(tagTile* currentTile)
//{
//	for (int i = 0; i < 3; i += 2)
//	{
//		for (int j = 0; j < 3; j += 2)
//		{
//			tagTile* tile = TotalMapTileV[(currentTile->Y - 1) * TILENUMX + (i * TILENUMX) + j];
//			
//			if (!tile->Opened) continue;
//			if (tile->Attribute == "start" || tile->Attribute == "wall") continue;
//			
//			tile->ParentTile = CurrentTile;
//			
//			bool addObj = 1;
//			for (OpenedTileVI = OpenedTileV.begin(); OpenedTileVI != OpenedTileV.end(); ++OpenedTileVI)
//			{
//				if ((*OpenedTileVI) == tile)
//				{
//					addObj = 0;
//					break;
//				}
//			}
//			
//			if (addObj) OpenedTileV.push_back(tile);
//		}
//	}
//
//	return OpenedTileV;
//}
//
//void AstarManager::PathFinder(tagTile* currentTile)
//{
//	float tempTotalCost = 50000;
//	tagTile* tempTile;
//
//	for (int i = 0; i < AddOpenedTile(currentTile).size(); i++)
//	{
//		OpenedTileV[i]->CostToGoal = ((abs(EndTile->X - OpenedTileV[i]->X) + abs(EndTile->Y - OpenedTileV[i]->Y)) * 10);
//		OpenedTileV[i]->CostFromStart = 10;
//		
//		OpenedTileV[i]->TotalCost = OpenedTileV[i]->CostToGoal + OpenedTileV[i]->CostFromStart;
//
//		if (tempTotalCost > OpenedTileV[i]->TotalCost)
//		{
//			tempTotalCost = OpenedTileV[i]->TotalCost;
//			tempTile = OpenedTileV[i];
//		}
//
//		bool addObj = 1;
//		for (OpenedTileVI = OpenedTileV.begin(); OpenedTileVI != OpenedTileV.end(); ++OpenedTileVI)
//		{
//			if (*OpenedTileVI == tempTile)
//			{
//				addObj = 0;
//				break;
//			}
//		}
//
//		OpenedTileV[i]->Opened = 0;
//		if (addObj) OpenedTileV.push_back(tempTile);
//	}
//
//	if (tempTile->Attribute == "end")
//	{
//		while (CurrentTile->ParentTile != NULL)
//		{
//			//CurrentTile->Color(RGB(22,14,128));
//			CurrentTile = CurrentTile->ParentTile;
//		}
//
//		return;
//	}
//
//	ClosedTileV.push_back(tempTile);
//
//	for (OpenedTileVI = OpenedTileV.begin(); OpenedTileVI != OpenedTileV.end(); ++OpenedTileVI)
//	{
//		if (*OpenedTileVI == tempTile)
//		{
//			OpenedTileVI = OpenedTileV.erase(OpenedTileVI);
//			break;
//		}
//	}
//
//	CurrentTile = tempTile;
//
//	if (OpenedTileV.size() == 0) return;
//
//	//PathFinder(CurrentTile);
//}