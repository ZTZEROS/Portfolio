#pragma once
#include "UI_Base.h"
#include "UI_Image.h"
#include "UI_Text.h"

class UI_Dialogue : public UI_Base
{
private:
	UI_Image* DialogueBox;

public:
	UI_Dialogue();
	~UI_Dialogue();
};

