#pragma once
#include "Lethita.h"


class CharacData
{
private:
	CharacData();
	CharacData(CharacData& other);
	~CharacData();


public:
	
	static CharacData& Get()
	{
		static CharacData instance;
		return instance;
	}
	
	void Initialize();
	void Destroy();

	//static inline void SetEqip(Item_Kind kind) { CharacData::Get().SetInstanceEqip(kind); }
	static inline void SetInfomation(CH_INFOMATION type, int num, char* name = nullptr) { CharacData::Get().SetInstanceInfomation(type, num, name); }
	static inline void SetEquipName(Item_Kind type, char* name) { CharacData::Get().SetEquipName(type, name); }//아이템 이름
	static inline void SetGameSpeed(float speed) { CharacData::Get().SetInstanceGameSpeed(speed); }//게임 스피드
	static inline void SetIsBoxRend(bool x) { CharacData::Get().SetInstanceIsBoxRend(x); }


	static inline int GetUnEqipInfo(CH_INFOMATION type) {return CharacData::Get().GetInstanceUnEqip(type); } //부위넘
	static inline int GetEqipInfo(CH_INFOMATION type) { return CharacData::Get().GetInstanceEqip(type); }
	static inline int GetBattleInfo(CH_INFOMATION type) { return CharacData::Get().GetInstanceBattle(type); }
	static inline char* GetEquip(Item_Kind kind) { return CharacData::Get().GetInstanceEquip( kind); }
	static inline Lethita* GetLethita() { return CharacData::Get().GetInstanceLethita(); }
	static inline float GetGameSpeed() { return CharacData::Get().GetInstanceGameSpeed(); }//게임 스피드
	static inline bool GetIsBoxRend() { return CharacData::Get().GetInstanceIsBoxRend(); }

protected:
	Lethita* player = nullptr;

	std::vector<int> chUnEquipInfo; //장비 미 착용시 능력치
	std::vector<int> chEquipInfo; //장비 착용시 능력치
	std::vector<int> chBattleInfo; //전투시 능력치
	char* chEquipName[Item_End]; //장착중인 아이템 이름
	
	float gameSpeed = 2.0f; //게임 전체적인 스피드
	bool isBoxRend = false;

	void InfoUpdate();
//	void SetInstanceEqip(Item_Kind kind);//장비 착용할 떄, 변경할 타입의 정보와 변경치
	void SetInstanceInfomation(CH_INFOMATION type, int num, char* name = nullptr);//그냥 정보를 업데이트 할 때
	inline void SetInstanceEquipName(Item_Kind type, char* name) { chEquipName[type] = name; };//아이템 이름
	inline void SetInstanceGameSpeed(float speed) { gameSpeed = speed; };//게임 스피드
	inline void SetInstanceIsBoxRend(bool x) { isBoxRend = x; };//게임 스피드
	
	inline float GetInstanceGameSpeed() { return gameSpeed; } //정보 출력
	inline int GetInstanceEqip(CH_INFOMATION type) { return chEquipInfo[type]; } //정보 출력
	inline int GetInstanceUnEqip(CH_INFOMATION type) { return chUnEquipInfo[type]; } //정보 출력
	inline int GetInstanceBattle(CH_INFOMATION type) { return chBattleInfo[type]; } //정보 출력
	inline char* GetInstanceEquip(Item_Kind kind) { return chEquipName[kind]; }
	inline bool GetInstanceIsBoxRend() { return isBoxRend; }
	inline Lethita* GetInstanceLethita() { return player; }

};

