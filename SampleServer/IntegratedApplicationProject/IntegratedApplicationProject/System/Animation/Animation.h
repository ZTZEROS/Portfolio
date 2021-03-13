#pragma once

enum CANCELABILITY_INDEX
{
	NO_CANCELABILITY,
	LOW_CANCELABILITY,
	MEDIUM_CANCELABILITY, //NORMAL_CANCELABILITY,
	HIGH_CANCELABILITY,
	CANCELABILITY_COUNT
};

enum REPLAY_OPTION_INDEX
{
	OFF_REPLAY_OPTION,
	ON_REPLAY_OPTION,
	REPLAY_OPTION_COUNT
};

typedef vector<UINT> UINT_V;
typedef vector<UINT>::iterator UINT_VI;

typedef vector<CANCELABILITY_INDEX> CANCELABILITY_INDEX_V;
typedef vector<CANCELABILITY_INDEX>::iterator CANCELABILITY_INDEX_VI;

typedef vector<BitmapImage*> BitmapImageAddressV;
typedef vector<BitmapImage*>::iterator BitmapImageAddressVI;

typedef vector<BitmapImage> BitmapImageV;
typedef vector<BitmapImage>::iterator BitmapImageVI;

class Animation
{
private:
	BitmapImageAddressV FrameImageV;
	UINT_V FrameIntervalV; //per Frame
	CANCELABILITY_INDEX_V CancelabilityV;

	UINT CurrentFrameInterval;
	UINT CurrentFrameImageIndex;

	REPLAY_OPTION_INDEX CurrentReplayOption;

	BOOL Playing;

public:
	Animation();
	~Animation();

	void AddFrameImage(BitmapImage* frameImageAddress, UINT FrameInterval, CANCELABILITY_INDEX cancelability, REPLAY_OPTION_INDEX replayOptionIndex = OFF_REPLAY_OPTION);

	void Update();
	void Render(UINT positionX = 0, UINT positionY = 0);
	
	void StartAnimation();

	BOOL CheckPlaying() { return Playing; }
	BOOL CheckLastFrame() { return (CurrentFrameImageIndex == FrameImageV.size() - 1) && (CurrentFrameInterval == FrameIntervalV[CurrentFrameImageIndex] - 1); }

	REPLAY_OPTION_INDEX GetCurrentReplayOption() { return CurrentReplayOption; }
	CANCELABILITY_INDEX GetCancelability() { return CancelabilityV[CurrentFrameImageIndex]; }

	void ResetCurrentFrameImageIndex() { CurrentFrameImageIndex = 0; }
};