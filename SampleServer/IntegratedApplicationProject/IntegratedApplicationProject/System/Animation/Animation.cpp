#include "stdafx.h"
#include "Animation.h"

Animation::Animation()
{
	CurrentFrameInterval = 0;
	CurrentFrameImageIndex = 0;
	CurrentReplayOption = OFF_REPLAY_OPTION;

	Playing = 0;
}

Animation::~Animation()
{
	for (unsigned int i = 0; i < FrameImageV.size(); ++i)
	{
		delete FrameImageV[i];
	}

	FrameImageV.clear();
	FrameIntervalV.clear();

	CurrentFrameInterval = 0;
	CurrentFrameImageIndex = 0;
	CurrentReplayOption = OFF_REPLAY_OPTION;

	Playing = 0;
}

void Animation::AddFrameImage(BitmapImage* frameImageAddress, UINT frameInterval, CANCELABILITY_INDEX cancelability, REPLAY_OPTION_INDEX replayOptionIndex)
{
	FrameImageV.push_back(frameImageAddress);
	FrameIntervalV.push_back(frameInterval);
	CancelabilityV.push_back(cancelability);
	CurrentReplayOption = replayOptionIndex;
}

void Animation::Update()
{
	if (Playing)
	{
		++CurrentFrameInterval %= FrameIntervalV[CurrentFrameImageIndex];

		if (!CurrentFrameInterval)
		{
			++CurrentFrameImageIndex %= FrameImageV.size();

			if (!CurrentFrameImageIndex)
			{
				if (CurrentReplayOption == OFF_REPLAY_OPTION)
				{
					Playing = 0;

					CurrentFrameInterval = 0;
					CurrentFrameImageIndex = 0;
				}
			}

			
		}
	}
}

void Animation::Render(UINT positionX, UINT positionY)
{
	FrameImageV[CurrentFrameImageIndex]->Render(positionX, positionY);
	
	//FrameImageV[CurrentFrameImageIndex]->Render(&BITMAP_BACK_BUFFER, positionX, positionY);

	//BITMAP_BACK_BUFFER.Render(FrameImageV[CurrentFrameImageIndex], positionX, positionY);
}

void Animation::StartAnimation()
{
	Playing = 1;

	CurrentFrameInterval = 0;
	CurrentFrameImageIndex = 0;
}