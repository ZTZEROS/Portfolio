#include "stdafx.h"
#include "RandomUtil.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>		/* time */

RandomUtil::RandomUtil()
{
}


RandomUtil::~RandomUtil()
{
}

void RandomUtil::Initialize()
{
	//�õ尪 �ʱ�ȭ
	srand((unsigned int)time(nullptr));
}

// min = 0, max = 10�̸� 0~10 �� �ϳ��� ���ڰ� �������� ��������.
int RandomUtil::GetInteger(int max, int min /*= 0*/)
{
	return min + (rand() % (max - min + 1));
}

// min = 0.0, max = 1.0�̸� 0.0~1.0 �� �ϳ��� ���ڰ� �������� ��������.
float RandomUtil::GetFloat(float max, float min /*= 0*/)
{
	return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

void RandomUtil::GetVector(D3DXVECTOR3& out, D3DXVECTOR3& max, D3DXVECTOR3& min)
{
	out.x = GetFloat(max.x, min.x);
	out.y = GetFloat(max.y, min.y);
	out.z = GetFloat(max.z, min.z);
}