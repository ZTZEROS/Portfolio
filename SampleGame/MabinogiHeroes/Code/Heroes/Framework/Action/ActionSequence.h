#pragma once
#include "ActionBase.h"
class ActionSequence :
	public ActionBase, public ActionDelegate
{
public:
	ActionSequence();
	virtual ~ActionSequence();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Destroy() override;

	virtual void AddAction(ActionBase* action);

	virtual void OnActionFinish(ActionBase* sender) override;
protected:
	std::vector<ActionBase*> actionArray;
	UINT32 currentIndex = 0;
};

