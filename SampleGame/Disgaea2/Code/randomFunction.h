#pragma once
#include "singletonBase.h"
#include <time.h>

//=======================================
// ## 2015. 10. 21 ## randomFunction ##
//=======================================

class randomFunction : public singletonBase < randomFunction >
{
public:
	randomFunction(void)
	{
		srand(GetTickCount());
	}
	~randomFunction(void)
	{

	}

	//int형 랜덤값 가져와보세!!
	inline int getInt(int num) { return rand() % num; }
	inline int getFromIntTo(int fromNum, int toNum) { return rand() % (toNum - fromNum + 1) + fromNum; }

	//float형 랜덤값 가져와보기
	float getFloat() { return (float)rand() / (float)RAND_MAX; }
	float getFloat(float num) { return ((float)rand() / (float)RAND_MAX) * num; }
	float getFromFloatTo(float fromFloat, float toFloat)
	{
		float rnd = (float)rand() / (float)RAND_MAX;

		return (rnd * (toFloat - fromFloat) + fromFloat);
	}


};