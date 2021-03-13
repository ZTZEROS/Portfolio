#pragma once
#include "../NPC.h"

class Eire : public NPC
{
private:
	enum CurrentFacialExpression { EIRE_NORMAL, EIRE_FACIAL_EXPRESSION_COUNT };
	enum CurrentDialogue { DIALOGUE_0, DIALOGUE_1, DIALOGUE_2, DIALOGUE_3, DIALOGUE_4, DIALOGUE_5, DIALOGUE_6, DIALOGUE_7, DIALOGUE_8, DIALOGUE_9, DIALOGUE_10,
		DIALOGUE_11, DIALOGUE_12, DIALOGUE_13, DIALOGUE_14, DIALOGUE_15, DIALOGUE_16, DIALOGUE_17, DIALOGUE_18, DIALOGUE_19, DIALOGUE_20, DIALOGUE_COUNT };

private:
	const char* EIRE_DIRECTORY = "./Eire/";

public:
	Eire();
	virtual ~Eire();

	virtual void Initialize() override;
	//virtual void Destroy() override;
	virtual void Reset() override;
	virtual void Update() override;
	virtual void Render() override;

};

