#pragma once
#include "ActionBase.h"
class ActionMove :
	public ActionBase
{
public:
	ActionMove();
	virtual ~ActionMove();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Destroy() override;

	inline void SetStart(const D3DXVECTOR3& _start){ start = _start; }
	inline void SetGoal(const D3DXVECTOR3& _goal){ goal = _goal; }
	inline void SetDuration(float duration){ durationTime = duration; }

protected:
	D3DXVECTOR3 start = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 goal = D3DXVECTOR3(0, 0, 0);

	float passedTime = 0.0f;
	float durationTime = 0.0f;
};

