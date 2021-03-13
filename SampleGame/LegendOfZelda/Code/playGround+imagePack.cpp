#include "stdafx.h"
#include "playGround.h"

void playGround::imagePack()
{
	//배경이미지
	IMAGEMANAGER->addImage("black", "../black.bmp", WINSIZEX, WINSIZEY, false, MAGENTA);
	IMAGEMANAGER->addImage("선택화면", "../mapSprites/characterSelect.bmp", WINSIZEX, WINSIZEY, false, MAGENTA);
	IMAGEMANAGER->addImage("성1층", "../mapSprites/castle1F.bmp", 1232 * IMAGESCALE, 1006 * IMAGESCALE, false, MAGENTA);
	IMAGEMANAGER->addImage("성2층", "../mapSprites/castle2ndFloor.bmp", 447 * IMAGESCALE, 496 * IMAGESCALE, false, MAGENTA);
	IMAGEMANAGER->addImage("요정", "../zeldaUI/naming/name_fairy.bmp", 32 * IMAGESCALE, 17 * IMAGESCALE, true, MAGENTA);
	IMAGEMANAGER->addImage("집", "../mapSprites/linkHouseDay.bmp", 256*IMAGESCALE, 224*IMAGESCALE, false, MAGENTA);
	IMAGEMANAGER->addImage("밤", "../mapSprites/darkBlue.bmp", WINSIZEX, WINSIZEY, false, MAGENTA, true);
	IMAGEMANAGER->addImage("집밖", "../mapSprites/outsideHouse.bmp", 1022 * IMAGESCALE, 1535 * IMAGESCALE, false, MAGENTA);
	IMAGEMANAGER->addImage("이름", "../mapSprites/makeName.bmp", WINSIZEX, WINSIZEY, false, MAGENTA);
	IMAGEMANAGER->addImage("알파벳", "../zeldaUI/naming/naming.bmp", 471 * IMAGESCALE, 62 * IMAGESCALE, true, MAGENTA);
	IMAGEMANAGER->addImage("세로선", "../zeldaUI/naming/dotLine.bmp", 1 * IMAGESCALE, 78 * IMAGESCALE, true, MAGENTA);
	IMAGEMANAGER->addImage("가로선", "../zeldaUI/naming/horizontalLine.bmp", 208 * IMAGESCALE, 1 * IMAGESCALE, true, MAGENTA);
	IMAGEMANAGER->addImage("이름지우기", "../mapSprites/deletePlayer.bmp", WINSIZEX, WINSIZEY, false, MAGENTA);
	IMAGEMANAGER->addImage("하수구", "../mapSprites/getSwordMap.bmp", 477 * IMAGESCALE, 431 * IMAGESCALE, false, MAGENTA);
	//효과
	IMAGEMANAGER->addFrameImage("검은원", "../mapSprites/gameOverEffect.bmp", 2040 * 7, 223 * 7,8,1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("비온다", "../mapSprites/rainingBright.bmp", 1024 * IMAGESCALE, 224 * IMAGESCALE, 4, 1, true, MAGENTA);
	//픽셀층~~~%_%
	IMAGEMANAGER->addImage("집안픽셀", "../pixelCheung/pixelLinkHouseInside.bmp", 256 * IMAGESCALE, 224 * IMAGESCALE, false, MAGENTA);
	IMAGEMANAGER->addImage("집밖픽셀", "../pixelCheung/pixeloutsideCastle.bmp", 1022 * IMAGESCALE, 1535 * IMAGESCALE, false, MAGENTA);
	IMAGEMANAGER->addImage("하수구픽셀", "../pixelCheung/swordmanPixel.bmp", 477 * IMAGESCALE, 431 * IMAGESCALE, false, MAGENTA);
	//알파벳
	for (int i = 65; i < 91; i++)
	{
		char* str;
		str = new char[26];
		char* str2;
		str2 = new char[26];
		sprintf(str, "../zeldaUI/naming/L_%c.bmp", i);
		sprintf(str2, "%c", i);
		IMAGEMANAGER->addImage(str2,str, 7 * IMAGESCALE, 15 * IMAGESCALE, true,MAGENTA);
		delete[] str;
		delete[] str2;
	}

	//오브젝트
	IMAGEMANAGER->addFrameImage("보물상자", "../ObjectSprites/chestBox.bmp", 32 * IMAGESCALE, 16 * IMAGESCALE, 2,1,false, MAGENTA);
	IMAGEMANAGER->addFrameImage("항아리", "../ObjectSprites/jarFrame.bmp", 32 * IMAGESCALE, 16 * IMAGESCALE, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addImage("항아리던지기용", "../ObjectSprites/Jar.bmp", 16 * 3, 16 * 3,  true, MAGENTA);
	IMAGEMANAGER->addFrameImage("항아리부서짐", "../ObjectSprites/BeingBrokenJar.bmp", 256 * 3, 48 * 3, 8, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("풀", "../ObjectSprites/bushFrame.bmp", 32 * IMAGESCALE, 16 * IMAGESCALE, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("풀던지기용", "../ObjectSprites/bush.bmp", 16 * 3, 16 * 3, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("풀날리기", "../ObjectSprites/BeingBrokenBush.bmp", 256 * IMAGESCALE, 48 * IMAGESCALE, 8, 1, true, MAGENTA);
	
	//UI Images 
	IMAGEMANAGER->addImage("LinkHead", "../zeldaUI/LinkHead.bmp", 16 * IMAGESCALE, 16 * IMAGESCALE, 1, MAGENTA);
	IMAGEMANAGER->addImage("InventoryCursor", "../zeldaUI/Inven/select_item.bmp", 32 * IMAGESCALE, 32 * IMAGESCALE, 1, MAGENTA);

	IMAGEMANAGER->addImage("StatusDisplay", "../zeldaUI/StatusDisplay.bmp", WINSIZEX, WINSIZEY, 1, MAGENTA);
	IMAGEMANAGER->addImage("MagicGauge", "../zeldaUI/gauge.bmp", 60, 140, 1, MAGENTA);
	IMAGEMANAGER->addImage("MagicPower", "../zeldaUI/gauge_in.bmp", 34, 108, 1, MAGENTA);
	IMAGEMANAGER->addImage("Heart_Full", "../zeldaUI/Heart_Full.bmp", 7 * IMAGESCALE, 7 * IMAGESCALE, 1, MAGENTA);
	IMAGEMANAGER->addImage("Heart_Half", "../zeldaUI/heart_Half.bmp", 7 * IMAGESCALE, 7 * IMAGESCALE, 1, MAGENTA);
	IMAGEMANAGER->addImage("Heart_Empty", "../zeldaUI/heart_Empty.bmp", 7 * IMAGESCALE, 7 * IMAGESCALE, 1, MAGENTA);
	IMAGEMANAGER->addImage("TextBox", "../zeldaUI/TXT_box.bmp", 756, 209, 1, MAGENTA);
	IMAGEMANAGER->addImage("SubweaponSlot", "../zeldaUI/main_equip_box_1.bmp", 80 * IMAGESCALE / 2, 72 * IMAGESCALE / 2, 1, MAGENTA);

	IMAGEMANAGER->addImage("Inventory", "../zeldaUI/Inven/Inventory.bmp", WINSIZEX, WINSIZEY, 1, MAGENTA);
	IMAGEMANAGER->addImage("ItemBox", "../zeldaUI/Inven/item_box.bmp", 152, 120, 1, MAGENTA);
	IMAGEMANAGER->addImage("DoBox", "../zeldaUI/Inven/do_box.bmp", 152, 72, 1, MAGENTA);
	IMAGEMANAGER->addImage("ItemEquipBox", "../zeldaUI/Inven/item_equip_box.bmp", 80, 48, 1, MAGENTA);
	IMAGEMANAGER->addImage("EquipBox", "../zeldaUI/Inven/equip_box_2.bmp", 80, 72, 1, MAGENTA);
	IMAGEMANAGER->addImage("PendentBox", "../zeldaUI/Inven/pendentif_box.bmp", 80, 72, 1, MAGENTA);

	IMAGEMANAGER->addImage("WorldMinimap", "../zeldaUI/Minimap/WorldMinimap.bmp", 510 * IMAGESCALE, 512 * IMAGESCALE, 1, MAGENTA);
	IMAGEMANAGER->addImage("DungeonMinimapSurface", "../zeldaUI/Minimap/DungeonMinimapSurface.bmp", WINSIZEX, WINSIZEY, 1, MAGENTA);
	IMAGEMANAGER->addImage("DungeonMinimapDepth", "../zeldaUI/Minimap/DungeonMinimapDepth.bmp", WINSIZEX, WINSIZEY, 1, MAGENTA);
	IMAGEMANAGER->addImage("DungeonMinimap2FtoB3", "../zeldaUI/Minimap/Dungeon2FtoB3.bmp", 112 * IMAGESCALE, float(480 * 24 / 7), 1, MAGENTA);
	IMAGEMANAGER->addImage("LocationAlert", "../zeldaUI/Minimap/LocationAlert.bmp", 21 * IMAGESCALE, float(117 * 24 / 7), 1, MAGENTA);
	//UI Images

	// 링크 이미지 모음//
	IMAGEMANAGER->addFrameImage("grassLiftDown", "../link/grassLiftDown.bmp", 204 * IMAGESCALE, 40 * IMAGESCALE, 6, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("grassLiftLeftandRight", "../link/grassLiftLeftandRight.bmp", 216 * IMAGESCALE, 70 * IMAGESCALE, 6, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("grassLiftMoveDown", "../link/grassLiftMoveDown.bmp", 80 * IMAGESCALE, 34 * IMAGESCALE, 5, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("grassLiftMoveLeftandRight", "../link/grassLiftMoveLeftandRight.bmp", 68 * IMAGESCALE, 70 * IMAGESCALE, 4, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("grassLiftMoveUp", "../link/grassLiftMoveUp.bmp", 112 * IMAGESCALE, 35 * IMAGESCALE, 7, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("grassLiftUp", "../link/grassLiftUp.bmp", 192 * IMAGESCALE, 34 * IMAGESCALE, 6, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkAttackDown", "../link/linkAttackDown.bmp", 204 * IMAGESCALE, 35 * IMAGESCALE, 6, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkAttackLeftandRight", "../link/linkAttackLeftandRight.bmp", 306 * IMAGESCALE, 66 * IMAGESCALE, 9, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkAttackUp", "../link/linkAttackUp.bmp", 324 * IMAGESCALE, 35 * IMAGESCALE, 9, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkBedJump", "../link/linkBedJump.bmp", 833 * IMAGESCALE, 38 * IMAGESCALE, 17, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkBoomerangDown", "../link/linkBoomerangDown.bmp", 81 * IMAGESCALE, 23 * IMAGESCALE, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkBoomerangLeftandRight", "../link/linkBoomerangLeftandRight.bmp", 66 * IMAGESCALE, 46 * IMAGESCALE, 3, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkBoomerangUp", "../link/linkBoomerangUp.bmp", 75 * IMAGESCALE, 23 * IMAGESCALE, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkChargeAttackLeft", "../link/linkChargeAttackLeft.bmp", 1040 * IMAGESCALE, 48 * IMAGESCALE, 20, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkChargeAttackRight", "../link/linkChargeAttackRight.bmp", 1000 * IMAGESCALE, 48 * IMAGESCALE, 20, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkChargeAttackUpandDown", "../link/linkChargeAttackUpandDown.bmp", 960 * IMAGESCALE, 104 * IMAGESCALE, 20, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkHit", "../link/linkHit.bmp", 80 * IMAGESCALE, 21 * IMAGESCALE, 4, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkHoleDrop", "../link/linkHoleDrop.bmp", 132 * IMAGESCALE, 21 * IMAGESCALE, 6, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkMoveLeftandRight", "../link/linkMoveLeftandRight.bmp", 119 * IMAGESCALE, 48 * IMAGESCALE, 7, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkMoveUpandDown", "../link/linkMoveUpandDown.bmp", 112 * IMAGESCALE, 48 * IMAGESCALE, 7, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkNonLiftLeftandRight", "../link/linkNonLiftLeftandRight.bmp", 100 * IMAGESCALE, 42 * IMAGESCALE, 4, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkNonLiftUpandDown", "../link/linkNonLiftUpandDown.bmp", 150 * IMAGESCALE, 44 * IMAGESCALE, 5, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkPushDown", "../link/linkPushDown.bmp", 48 * IMAGESCALE, 20 * IMAGESCALE, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkPushLeftandRight", "../link/linkPushLeftandRight.bmp", 64 * IMAGESCALE, 46 * IMAGESCALE, 4, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkPushUp", "../link/linkPushUp.bmp", 48 * IMAGESCALE, 23 * IMAGESCALE, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkSleep", "../link/linkSleep.bmp", 750 * IMAGESCALE, 42 * IMAGESCALE, 25, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkStand", "../link/linkStand.bmp", 80 * IMAGESCALE, 46 * IMAGESCALE, 4, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkStandDownCharge", "../link/linkStandDownCharge.bmp", 300 * IMAGESCALE, 34 * IMAGESCALE, 15, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkStandLeftandRightCharge", "../link/linkStandLeftandRightCharge.bmp", 812 * IMAGESCALE, 46 * IMAGESCALE, 28, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkStandUpCharge", "../link/linkStandUpCharge.bmp", 304 * IMAGESCALE, 33 * IMAGESCALE, 16, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkWaterWave", "../link/linkWaterWave.bmp", 48 * IMAGESCALE, 8 * IMAGESCALE, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkWeaponMoveDown", "../link/linkWeaponMoveDown.bmp", 112 * IMAGESCALE, 24 * IMAGESCALE, 7, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkWeaponMoveLeftandRight", "../link/linkWeaponMoveLeftandRight.bmp", 133 * IMAGESCALE, 48 * IMAGESCALE, 7, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("linkWeaponMoveUp", "../link/linkWeaponMoveUp.bmp", 119 * IMAGESCALE, 24 * IMAGESCALE, 7, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("swordTipTwinkelUpandDown", "../link/swordTipTwinkelUpandDown.bmp", 336 * IMAGESCALE, 7 * IMAGESCALE, 21, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("swordTipTwinkleLeftandRight", "../link/swordTipTwinkleLeftandRight.bmp", 168 * IMAGESCALE, 21 * IMAGESCALE, 21, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("potLiftDown", "../link/potLiftDown.bmp", 204 * IMAGESCALE, 38 * IMAGESCALE, 6, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("potLiftLeftandRight", "../link/potLiftLeftandRight.bmp", 198 * IMAGESCALE, 66 * IMAGESCALE, 6, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("potLiftMoveDown", "../link/potLiftMoveDown.bmp", 96 * IMAGESCALE, 33 * IMAGESCALE, 6, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("potLiftMoveLeftandRight", "../link/potLiftMoveLeftandRight.bmp", 68 * IMAGESCALE, 66 * IMAGESCALE, 4, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("potLiftMoveUp", "../link/potLiftMoveUp.bmp", 112 * IMAGESCALE, 33 * IMAGESCALE, 7, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("potLiftUp", "../link/potLiftUp.bmp", 160 * IMAGESCALE, 31 * IMAGESCALE, 5, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("potLiftStand", "../link/potLiftStand.bmp", 80 * IMAGESCALE, 33 * IMAGESCALE, 4, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("fullChargeStand", "../link/fullChargeStand.bmp", 112 * IMAGESCALE, 33 * IMAGESCALE, 4, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("liftGrassStand", "../link/liftGrassStand.bmp", 80 * IMAGESCALE, 35 * IMAGESCALE, 4, 1, true, MAGENTA);
	// 링크 이미지 모음

	// 링크 세이브 이미지
	IMAGEMANAGER->addImage("linkNormalSaveImg", "../link/linkNormalSaveImg.bmp", 16, 21, true, MAGENTA);
	IMAGEMANAGER->addImage("linkWeaponSaveImg", "../link/linkWeaponSaveImg.bmp", 20, 27, true, MAGENTA);
	// 링크 세이브 이미지

	// NPC 이미지 모음
	IMAGEMANAGER->addImage("priestUp", "../NPC/priest_up.bmp", 16 * IMAGESCALE, 24 * IMAGESCALE, true, MAGENTA);
	IMAGEMANAGER->addImage("priestDown", "../NPC/priest_down.bmp", 16 * IMAGESCALE, 24 * IMAGESCALE, true, MAGENTA);
	IMAGEMANAGER->addImage("priestLeft", "../NPC/priest_left.bmp", 16 * IMAGESCALE, 24 * IMAGESCALE, true, MAGENTA);
	IMAGEMANAGER->addImage("priestRight", "../NPC/priest_right.bmp", 16 * IMAGESCALE, 24 * IMAGESCALE, true, MAGENTA);

	IMAGEMANAGER->addFrameImage("princessUp", "../NPC/princess_up.bmp", 30 * IMAGESCALE, 23 * IMAGESCALE, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("princessDown", "../NPC/princess_down.bmp", 32 * IMAGESCALE, 23, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("princessLeft", "../NPC/princess_left.bmp", 28 * IMAGESCALE, 23, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("princessRight", "../NPC/princess_right.bmp", 28 * IMAGESCALE, 23 * IMAGESCALE, 2, 1, true, MAGENTA);

	IMAGEMANAGER->addFrameImage("soldierUp", "../NPC/soldier_up.bmp", 25 * IMAGESCALE, 28 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("soldierDown", "../NPC/soldier_down.bmp", 25 * IMAGESCALE, 30 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("soldierLeft", "../NPC/soldier_left.bmp", 21 * IMAGESCALE, 28 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("soldierRight", "../NPC/soldier_right.bmp", 21 * IMAGESCALE, 28 * IMAGESCALE, 1, 1, true, MAGENTA);

	IMAGEMANAGER->addFrameImage("uncleDown", "../NPC/uncle_walk_down.bmp", 60 * IMAGESCALE, 27 * IMAGESCALE, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("uncleLeft", "../NPC/uncle_walk_left.bmp", 42 * IMAGESCALE, 26 * IMAGESCALE, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("uncleRight", "../NPC/uncle_walk_right.bmp", 42 * IMAGESCALE, 26 * IMAGESCALE, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("uncleSit", "../NPC/uncle_sit_down.bmp", 16 * IMAGESCALE, 23 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("uncleSitLeft", "../NPC/uncle_sit_left.bmp", 16 * IMAGESCALE, 23 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("uncleWeapon", "../NPC/uncle_die_weapon.bmp", 28 * IMAGESCALE, 28 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("uncleDie", "../NPC/uncle_die.bmp", 26 * IMAGESCALE, 24 * IMAGESCALE, true, 1, 1, MAGENTA);
	// NPC 이미지 모음

	// ITEM 이미지 모음
	//인벤, 장비 아이템, 던전 아이템
	IMAGEMANAGER->addFrameImage("sword_01", "../zeldaUI/Inven/equip_item/equip_item_01.bmp", 16 * IMAGESCALE, 16 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("shield_01", "../zeldaUI/Inven/equip_item/equip_item_05.bmp", 16 * IMAGESCALE, 16 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("inven_map", "../zeldaUI/Inven/equip_item/equip_item_16.bmp", 16 * IMAGESCALE, 16 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("inven_bigkey", "../zeldaUI/Inven/equip_item/equip_item_18.bmp", 16 * IMAGESCALE, 16 * IMAGESCALE, 1, 1, true, MAGENTA);
	//인벤 서브 아이템
	IMAGEMANAGER->addFrameImage("inven_boomerang", "../zeldaUI/Inven/item/item_02.bmp", 16 * IMAGESCALE, 16 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("inven_lamp", "../zeldaUI/Inven/item/item_11.bmp", 16 * IMAGESCALE, 16 * IMAGESCALE, 1, 1, true, MAGENTA);
	//인벤 서브 아이템Frame 이름
	IMAGEMANAGER->addFrameImage("name_boomerang", "../zeldaUI/Inven/item/BoomerangNameplate.bmp", 72 * IMAGESCALE, 40 * 24 / 7, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("name_lamp", "../zeldaUI/Inven/item/LampNameplate.bmp", 72 * IMAGESCALE, 40 * 24 / 7, 1, 1, true, MAGENTA);
	//드랍 UI 아이템	 Frame
	IMAGEMANAGER->addFrameImage("heart_half", "../zeldaUI/Inven/item/half_heart.bmp", 16 * IMAGESCALE, 16 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("heart_one", "../zeldaUI/Inven/item/one_heart.bmp", 16 * IMAGESCALE / 2, 16 * IMAGESCALE / 2, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("small_key", "../zeldaUI/Inven/item/small_key.bmp", 16 * IMAGESCALE, 16 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("Red_Crystal", "../zeldaUI/Inven/item/RedCrystal.bmp", 48 * IMAGESCALE, 16 * IMAGESCALE, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("Green_Crystal", "../zeldaUI/Inven/item/GreenCrystal.bmp", 48 * IMAGESCALE, 16 * IMAGESCALE, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("Purple_Crystal", "../zeldaUI/Inven/item/PurpleCrystal.bmp", 48 * IMAGESCALE, 16 * IMAGESCALE, 3, 1, true, MAGENTA);
	//드랍 서브 아이템, 던전 아이템
	IMAGEMANAGER->addFrameImage("map", "../zeldaUI/Inven/item/map.bmp", 16 * IMAGESCALE, 16 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("big_key", "../zeldaUI/Inven/item/master_key.bmp", 16 * IMAGESCALE, 17 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("boomerang", "../zeldaUI/Inven/item/BoomerangOnMap.bmp", 16 * IMAGESCALE, 16 * IMAGESCALE, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("lamp", "../zeldaUI/Inven/item/LampOnMap.bmp", 16 * IMAGESCALE, 16 * IMAGESCALE, 1, 1, true, MAGENTA);
	// ITEM 이미지 모음

	// 몬스터 이미지 모음
	//IMAGEMANAGER->addFrameImage("greensoldier", "../NPC/greensoldier.bmp", 128, 128, 4, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("greensoldierattackleft", "../NPC/greensoldier_attack_left.bmp", 384, 96, 4, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("greensoldierattackclimb", "../NPC/greensoldier_attack_climb.bmp", 384, 96, 4, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("greensoldierattackright", "../NPC/greensoldier_attack_right.bmp", 384, 96, 4, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("greensoldierattackdescend", "../NPC/greensoldier_attack_descend.bmp", 384, 96, 4, 1, true, MAGENTA);

	//IMAGEMANAGER->addFrameImage("bluesoldier", "../NPC/bluesoldier.bmp", 128, 152, 4, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("bluesoldierattackleft", "../NPC/bluesoldier_attack_left.bmp", 384, 114, 4, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("bluesoldierattackclimb", "../NPC/bluesoldier_attack_climb.bmp", 384, 114, 4, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("bluesoldierattackright", "../NPC/bluesoldier_attack_right.bmp", 384, 114, 4, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("bluesoldierattackdescend", "../NPC/bluesoldier_attack_descend.bmp", 384, 114, 4, 1, true, MAGENTA);
	//
	IMAGEMANAGER->addFrameImage("spider", "../NPC/spider.bmp", 270, 90, 3, 1, true, MAGENTA);
	////boss공백
	////실험
	IMAGEMANAGER->addFrameImage("ironmaceattackleft", "../NPC/ironmace_attack_left & right.bmp", 12600, 600, 42, 2, true, MAGENTA);
	//IMAGEMANAGER->addFrameImage("ironmaceattackright", "../NPC/ironmace_attack_left & right.bmp", 12600, 600, 42, 1, true, MAGENTA);
	////실험
	IMAGEMANAGER->addFrameImage("ironmacewalkleft", "../NPC/ironmace_walk_left.bmp", 180, 150, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("ironmacewalkright", "../NPC/ironmace_walk_right.bmp", 180, 150, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("iron", "../NPC/iron.bmp", 90, 90, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("ironconnect", "../NPC/ironconnect.bmp", 90, 90, 1, 1, true, MAGENTA);
	////boss공백
	IMAGEMANAGER->addFrameImage("mouseclimbstop", "../NPC/mouse_climb_stop.bmp", 90, 90, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("mousedescendstop", "../NPC/mouse_descend_stop.bmp", 90, 90, 1, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("mousewalkleft", "../NPC/mouse_walk_left.bmp", 180, 90, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("mousewalkclimb", "../NPC/mouse_walk_climb.bmp", 180, 90, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("mousewalkright", "../NPC/mouse_walk_right.bmp", 180, 90, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("mousewalkdescend", "../NPC/mouse_walk_descend.bmp", 180, 90, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("snakewalkleft", "../NPC/snake_walk_left.bmp", 0, 0, 180, 90, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("snakewalkclimb", "../NPC/snake_walk_climb.bmp", 0, 0, 180, 90, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("snakewalkright", "../NPC/snake_walk_right.bmp", 0, 0, 180, 90, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("snakewalkdescend", "../NPC/snake_walk_descend.bmp", 0, 0, 180, 90, 2, 1, true, MAGENTA);
	//IMAGEMANAGER->addFrameImage("spider", "../NPC/spider_walk.bmp", 270, 90, 3, 1, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("batmove", "../NPC/bat_move.bmp", 750, 150, 5, 1, true, MAGENTA);
	// 몬스터 이미지 모음

}