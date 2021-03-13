#pragma once
#include "../Util/Dijkstra.h"
#include "Barricade.h"
#include "PathNode.h"

class PathFinder :
	public Dijkstra
{
public:
	PathFinder();
	virtual ~PathFinder();

	virtual void Initialize(char* dataFile = nullptr) override;
	void Destroy();
	void Render();

	bool CheckBarricadeCollision(const D3DXVECTOR3& start, const D3DXVECTOR3& end);
	int FindNearestPathNode(const D3DXVECTOR3& pos);
	void PathOptimize(std::list<int>& pathList, const D3DXVECTOR3& start, const D3DXVECTOR3& end);

	inline const D3DXVECTOR3& GetPathNodePosition(int index){ return path[index]->GetPosition(); }

protected:
	std::vector<PathNode*> path;
	std::vector<Barricade*> barricades;

	std::vector<FVF_PositionColor> edgeLine;
	UINT lineCount = 0;

	static const int PATH_NODE_COUNT = 8;
	static const int BARRICADE_COUNT = 3;

};

