#pragma once

class EthernetProfiler
{
private:

public:
	EthernetProfiler();
	virtual ~EthernetProfiler();

	bool Initialize();
	bool Update();
	bool Render();
	bool Terminalize();
};
