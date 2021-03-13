#pragma once
class Skill
{
public:
	Skill();
	~Skill();

	inline void SetIsShow(bool set) { isShow = set; }

private:
	bool isShow = false;
};

