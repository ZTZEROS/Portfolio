#pragma once

//�����Լ��� �� �� ���ϰ� �������� ���� ��ƿ��Ƽ Ŭ����

class RandomUtil
{
public:
	RandomUtil();
	~RandomUtil();

	static void Initialize();

	//min~max �������� �������� int�� ���Ѵ�.
	static int GetInteger(int max, int min = 0);

	//min~max �������� �������� float�� ���Ѵ�.
	static float GetFloat(float max, float min = 0);

	static void GetVector(D3DXVECTOR3& out, D3DXVECTOR3& max, D3DXVECTOR3& min);
};

	