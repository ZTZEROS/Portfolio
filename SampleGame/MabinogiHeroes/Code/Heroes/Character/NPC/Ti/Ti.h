#pragma once
#include "../NPC.h"

class Ti : public NPC
{
private:
	enum CurrentFacialExpression { TI_NORMAL, TI_SMILE_NORMAL, TI_SMILE_TEETH, TI_MORRIGAHAN_NORMAL, TI_MORIGAHN_SAD, TI_MORRIGAHAN_SMILE, TI_NORMAL_XMAS, TI_SMILE_NORMAL_XMAS, TI_SMILE_TEETH_XMAS, TI_FACIAL_EXPRESSION_COUNT };
	enum CurrentDialogue { DIALOGUE_0, DIALOGUE_1, DIALOGUE_2, DIALOGUE_3, DIALOGUE_4, DIALOGUE_5, DIALOGUE_6, DIALOGUE_7, DIALOGUE_8, DIALOGUE_9, DIALOGUE_10, DIALOGUE_11, DIALOGUE_12, DIALOGUE_COUNT };

private:
	const char* TI_DIRECTORY = "./Ti/";

public:
	Ti();
	virtual ~Ti();

	virtual void Initialize() override;
	//virtual void Destroy() override;
	virtual void Reset() override;
	virtual void Update() override;
	virtual void Render() override;

	//virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void ItemSetting() override;
};

