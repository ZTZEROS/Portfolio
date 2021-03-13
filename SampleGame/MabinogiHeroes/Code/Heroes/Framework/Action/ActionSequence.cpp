#include "stdafx.h"
#include "ActionSequence.h"


ActionSequence::ActionSequence()
{
}


ActionSequence::~ActionSequence()
{
}

void ActionSequence::Start()
{
	if (actionArray.empty())
		return;

	currentIndex = 0;
	actionArray[currentIndex]->Start();
}

void ActionSequence::Update()
{
	if (currentIndex < actionArray.size())
	{
		actionArray[currentIndex]->Update();
	}
}

void ActionSequence::Destroy()
{
	for (size_t i = 0; i < actionArray.size(); ++i)
	{
		SAFE_DELETE(actionArray[i]);
	}
	actionArray.clear();
}

void ActionSequence::AddAction(ActionBase* action)
{
	if (action)
	{
		actionArray.push_back(action);
	}
}

void ActionSequence::OnActionFinish(ActionBase* sender)
{
	++currentIndex;
	if (currentIndex < actionArray.size())
	{
		actionArray[currentIndex]->Start();
	}
	else
	{
		if (actionDelegate)
		{
			actionDelegate->OnActionFinish(this);
		}
	}
}