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
	//edgeCost�� node ���̵� ����
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
		//�Ľ��ϴ°� ���ڰ� �˾Ƽ�
	}
	else
	{
		//���� ������ �׽�Ʈ�� ������ ���
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
	//�ʱ�ȭ
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
			if ( node[i].cost < minCost && node[i].visited == false )	// �湮 ���� �� �� �ִ� ����� �� �̱�
			{
				minCost = node[i].cost;
				minCostNodeID = i;
			}
		}

		if ( minCostNodeID < 0 )	//�湮 ���� ��尡 ����.
			break;

		node[minCostNodeID].visited = true;		//�켱 �湮�ߴٰ� üũ�ϰ�
		for ( UINT i = 0; i < NODE_COUNT; ++i )	//�湮���� ��� ��, �ּ� ��� ��尡 �� �� �ִ� �͵鸸, �ּҺ�� ����� �ڽ�Ʈ�� ����Ͽ� ����
		{
			if ( node[i].visited == true )
				continue;

			float newCost = edgeCost[minCostNodeID][i] + node[minCostNodeID].cost;
			if ( newCost < node[i].cost )
			{
				node[i].cost = newCost;
				node[i].prevNodeID = minCostNodeID;	//�� ��ΰ� ���� ��κ��� �ڽ�Ʈ�� ������ ��θ� �����Ѵ�.
			}
		}
	}

	out.clear();
	int tempID = end;
	while ( tempID != -1 )	//��ǥ �������� ���������� ���󰡸鼭 out����Ʈ �ϼ�
	{
		out.push_front(tempID);
		tempID = node[tempID].prevNodeID;
	}

	if ( out.front() != start )
	{
		out.clear();	//Ȥ�ó� �𸣴� Ȯ�� ����
	}
}