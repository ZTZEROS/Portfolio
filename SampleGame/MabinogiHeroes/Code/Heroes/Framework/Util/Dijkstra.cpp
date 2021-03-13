#include "stdafx.h"
#include "Dijkstra.h"


Dijkstra::Dijkstra()
{
	ZeroMemory(edgeCost, sizeof(float)*NODE_COUNT*NODE_COUNT);
}


Dijkstra::~Dijkstra()
{
}

void Dijkstra::Initialize(char* dataFile)
{
	//edgeCost와 node 아이디 세팅
	for ( UINT i = 0; i < NODE_COUNT; ++i )
	{
		for ( UINT j = 0; j < NODE_COUNT; ++j )
		{
			if ( i == j )
			{
				edgeCost[i][j] = 0.0f;				
			}
			else
			{
				edgeCost[i][j] = INFINITY_DISTANCE;
			}
		}

		node[i].nodeID = i;
	}

	if ( dataFile != nullptr )
	{
		//파싱하는건 각자가 알아서
	}
	else
	{
		//파일 없으면 테스트용 데이터 사용
		edgeCost[0][0] = 0.0f;	//A
		edgeCost[0][1] = 10.0f;	//A->B
		edgeCost[0][2] = 30.0f;	//A->C
		edgeCost[0][3] = 15.0f;	//A->D

		edgeCost[1][1] = 0.0f;	//B
		edgeCost[1][4] = 20.0f;	//B->E

		edgeCost[2][2] = 0.0f;	//C
		edgeCost[2][5] = 5.0f;	//C->F

		edgeCost[3][3] = 0.0f;	//D
		edgeCost[3][2] = 5.0f;	//D->C
		edgeCost[3][5] = 20.0f;	//D->F

		edgeCost[4][4] = 0.0f;	//E
		edgeCost[4][5] = 20.0f;	//E->F

		edgeCost[5][5] = 0.0f;	//F
		edgeCost[5][3] = 20.0f;	//F->D
	}	
}

void Dijkstra::PathFind(std::list<int>& out, int start, int end)
{
	//초기화
	for ( UINT i = 0; i < NODE_COUNT; ++i )
	{
		node[i].cost = INFINITY_DISTANCE;
		node[i].prevNodeID = -1;
		node[i].visited = false;
	}
	
	node[start].cost = 0;

	while ( true )
	{
		float minCost = INFINITY_DISTANCE;
		int minCostNodeID = -1;
		for ( UINT i = 0; i < NODE_COUNT; ++i )
		{
			if ( node[i].cost < minCost && node[i].visited == false )	// 방문 안한 것 중 최단 경로인 것 뽑기
			{
				minCost = node[i].cost;
				minCostNodeID = i;
			}
		}

		if ( minCostNodeID < 0 )	//방문 안한 노드가 없다.
			break;

		node[minCostNodeID].visited = true;		//우선 방문했다고 체크하고
		for ( UINT i = 0; i < NODE_COUNT; ++i )	//방문안한 노드 중, 최소 비용 노드가 갈 수 있는 것들만, 최소비용 노드의 코스트를 고려하여 갱신
		{
			if ( node[i].visited == true )
				continue;

			float newCost = edgeCost[minCostNodeID][i] + node[minCostNodeID].cost;
			if ( newCost < node[i].cost )
			{
				node[i].cost = newCost;
				node[i].prevNodeID = minCostNodeID;	//새 경로가 기존 경로보다 코스트가 적으면 경로를 갱신한다.
			}
		}
	}

	out.clear();
	int tempID = end;
	while ( tempID != -1 )	//목표 지점에서 이전노드들을 따라가면서 out리스트 완성
	{
		out.push_front(tempID);
		tempID = node[tempID].prevNodeID;
	}

	if ( out.front() != start )
	{
		out.clear();	//혹시나 모르니 확인 절차
	}
}