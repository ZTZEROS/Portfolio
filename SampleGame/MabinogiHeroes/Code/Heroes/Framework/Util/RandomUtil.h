#pragma once

//랜덤함수를 좀 더 편하게 쓰기위해 만든 유틸리티 클래스

class RandomUtil
{
public:
	RandomUtil();
	~RandomUtil();

	static void Initialize();

	//min~max 범위에서 랜덤으로 int를 구한다.
	static int GetInteger(int max, int min = 0);

	//min~max 범위에서 랜덤으로 float를 구한다.
	static float GetFloat(float max, float min = 0);

	static void GetVector(D3DXVECTOR3& out, D3DXVECTOR3& max, D3DXVECTOR3& min);
};

	