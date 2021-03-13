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
	static inline void SetEquipName(Item_Kind type, char* name) { CharacData::Get().SetEquipName(type, name); }//������ �̸�
	static inline void SetGameSpeed(float speed) { CharacData::Get().SetInstanceGameSpeed(speed); }//���� ���ǵ�
	static inline void SetIsBoxRend(bool x) { CharacData::Get().SetInstanceIsBoxRend(x); }


	static inline int GetUnEqipInfo(CH_INFOMATION type) {return CharacData::Get().GetInstanceUnEqip(type); } //������
	static inline int GetEqipInfo(CH_INFOMATION type) { return CharacData::Get().GetInstanceEqip(type); }
	static inline int GetBattleInfo(CH_INFOMATION type) { return CharacData::Get().GetInstanceBattle(type); }
	static inline char* GetEquip(Item_Kind kind) { return CharacData::Get().GetInstanceEquip( kind); }
	static inline Lethita* GetLethita() { return CharacData::Get().GetInstanceLethita(); }
	static inline float GetGameSpeed() { return CharacData::Get().GetInstanceGameSpeed(); }//���� ���ǵ�
	static inline bool GetIsBoxRend() { return CharacData::Get().GetInstanceIsBoxRend(); }

protected:
	Lethita* player = nullptr;

	std::vector<int> chUnEquipInfo; //��� �� ����� �ɷ�ġ
	std::vector<int> chEquipInfo; //��� ����� �ɷ�ġ
	std::vector<int> chBattleInfo; //������ �ɷ�ġ
	char* chEquipName[Item_End]; //�������� ������ �̸�
	
	float gameSpeed = 2.0f; //���� ��ü���� ���ǵ�
	bool isBoxRend = false;

	void InfoUpdate();
//	void SetInstanceEqip(Item_Kind kind);//��� ������ ��, ������ Ÿ���� ������ ����ġ
	void SetInstanceInfomation(CH_INFOMATION type, int num, char* name = nullptr);//�׳� ������ ������Ʈ �� ��
	inline void SetInstanceEquipName(Item_Kind type, char* name) { chEquipName[type] = name; };//������ �̸�
	inline void SetInstanceGameSpeed(float speed) { gameSpeed = speed; };//���� ���ǵ�
	inline void SetInstanceIsBoxRend(bool x) { isBoxRend = x; };//���� ���ǵ�
	
	inline float GetInstanceGameSpeed() { return gameSpeed; } //���� ���
	inline int GetInstanceEqip(CH_INFOMATION type) { return chEquipInfo[type]; } //���� ���
	inline int GetInstanceUnEqip(CH_INFOMATION type) { return chUnEquipInfo[type]; } //���� ���
	inline int GetInstanceBattle(CH_INFOMATION type) { return chBattleInfo[type]; } //���� ���
	inline char* GetInstanceEquip(Item_Kind kind) { return chEquipName[kind]; }
	inline bool GetInstanceIsBoxRend() { return isBoxRend; }
	inline Lethita* GetInstanceLethita() { return player; }

};

