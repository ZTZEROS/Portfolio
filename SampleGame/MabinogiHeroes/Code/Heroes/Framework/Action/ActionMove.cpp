#include "stdafx.h"
#include "ActionMove.h"


ActionMove::ActionMove()
{
}


ActionMove::~ActionMove()
{
}

void ActionMove::Start() 
{
	if (!target)
		return;

	isStart = true;
	passedTime = 0.0f;
	target->SetPosition(start);
	//각도 조정

	D3DXVECTOR3 a(0, 0, 1);
	D3DXVECTOR3 b = goal - start;

	float aLength = D3DXVec3Length(&a);
	float bLength = D3DXVec3Length(&b);
	float dot = D3DXVec3Dot(&a, &b);

	float angle = acos(dot / (aLength * bLength));

	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, &a, &b);
	if (cross.y < 0)
	{
		angle = -angle;
	}
	target->SetAngle(angle);
}

void ActionMove::Update()
{
	if (!target || isStart == false)
		return;

	passedTime += (float)GameManager::GetTick();

	if (passedTime >= durationTime)
	{
		//도착 종료
		target->SetPosition(goal);
		isStart = false;
		if (actionDelegate)
		{
			actionDelegate->OnActionFinish(this);
		}
		return;
	}

	float t = passedTime / durationTime;

	D3DXVECTOR3 newPosition;
	D3DXVec3Lerp(&newPosition, &start, &goal, t);
	target->SetPosition(newPosition);
}

void ActionMove::Destroy()
{
}
