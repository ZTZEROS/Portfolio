#pragma once
#include <list>

class DijkstraNode
{
public:
	DijkstraNode(){};
	~DijkstraNode(){};
	
	int nodeID = -1;
	int prevNodeID = -1;		//최단 경로일때 이전 노드의 ID
	
	float cost = 1000000000.0f;	//시작노드에서 이 노드까지 가는데 필요한 비용( 대략 절대로 안쓸 큰 숫자 )
	bool visited = false;		//이 노드를 방문했는지 안했는지
};

class Dijkstra
{
public:
	Dijkstra();
	~Dijkstra();

	virtual void Initialize(char* dataFile = nullptr);
	void PathFind(std::list<int>& out, int start, int end);

protected:
	const float INFINITY_DISTANCE = 1000000000.0f;

	static const UINT NODE_COUNT = 8;
	float edgeCost[NODE_COUNT][NODE_COUNT];

	DijkstraNode node[NODE_COUNT];
};

