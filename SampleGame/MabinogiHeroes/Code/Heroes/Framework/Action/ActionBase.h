#pragma once
#include "../Object/GameObject.h"

class ActionDelegate
{
public :
	virtual void OnActionFinish(ActionBase* sender) = 0;
};

class ActionBase
{
public:
	ActionBase();
	virtual ~ActionBase();

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;

	inline GameObject* GetTarget(){ return target; }
	inline void SetTarget(GameObject* _target){ target = _target; }

	inline void SetDelegate(ActionDelegate*  _delegate){ actionDelegate = _delegate; }
protected:
	GameObject* target = nullptr;
	bool isStart = false;

	ActionDelegate* actionDelegate = nullptr;
};

