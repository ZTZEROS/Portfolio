#include "stdafx.h"
#include "PathFinder.h"


PathFinder::PathFinder()
{
}


PathFinder::~PathFinder()
{
}

void PathFinder::Initialize(char* dataFile/* = nullptr*/)
{
	path.resize(PATH_NODE_COUNT);
	path[0] = new PathNode;
	path[0]->Initialize(D3DXVECTOR3(1.5f, 0.0f, -2.0f));
	path[1] = new PathNode;
	path[1]->Initialize(D3DXVECTOR3(-2.0f, 0.0f, 4.0f));
	path[2] = new PathNode;
	path[2]->Initialize(D3DXVECTOR3(0.0f, 0.0f, 11.0f));
	path[3] = new PathNode;
	path[3]->Initialize(D3DXVECTOR3(11.0f, 0.0f, 11.0f));
	path[4] = new PathNode;
	path[4]->Initialize(D3DXVECTOR3(12.0f, 0.0f, -2.0f));
	path[5] = new PathNode;
	path[5]->Initialize(D3DXVECTOR3(-4.0f, 0.0f, -7.0f));
	path[6] = new PathNode;
	path[6]->Initialize(D3DXVECTOR3(-9.0f, 0.0f, -3.0f));
	path[7] = new PathNode;
	path[7]->Initialize(D3DXVECTOR3(-9.0f, 0.0f, 4.0f));

	barricades.resize(BARRICADE_COUNT);
	barricades[0] = new Barricade;
	barricades[0]->Initialize(D3DXVECTOR3(5, 0, -10), 5);
	barricades[1] = new Barricade;
	barricades[1]->Initialize(D3DXVECTOR3(5, 0, 5), 5);
	barricades[2] = new Barricade;
	barricades[2]->Initialize(D3DXVECTOR3(-5, 0, 0), 3);

	ZeroMemory(edgeCost, sizeof(float)*PATH_NODE_COUNT* PATH_NODE_COUNT);
	for (int i = 0; i < PATH_NODE_COUNT; ++i)
	{
		for (int j = i + 1; j < PATH_NODE_COUNT; ++j)
		{
			const D3DXVECTOR3& start = path[i]->GetPosition();
			const D3DXVECTOR3& end = path[j]->GetPosition();
			bool isCollision = CheckBarricadeCollision(start, end);
			if (isCollision == false)
			{
				D3DXVECTOR3 distance = (end - start);
				float cost = D3DXVec3Length(&distance);
				edgeCost[i][j] = cost;
				edgeCost[j][i] = cost;

				FVF_PositionColor edgeStart = FVF_PositionColor(start, D3DCOLOR_XRGB(255, 0, 0));
				edgeStart.pos.y += 0.02f;
				FVF_PositionColor edgeEnd = FVF_PositionColor(end, D3DCOLOR_XRGB(255, 0, 0));
				edgeEnd.pos.y += 0.02f;
				edgeLine.push_back(edgeStart);
				edgeLine.push_back(edgeEnd);
			}
			else
			{
				edgeCost[i][j] = INFINITY_DISTANCE;
				edgeCost[j][i] = INFINITY_DISTANCE;
			}
		}
	}
	lineCount = (UINT)(edgeLine.size()*0.5f);

}

void PathFinder::Destroy()
{
	for (auto iter = path.begin(); iter != path.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	path.clear();
	
	//for (auto iter : barricades)
	//{
	//	SAFE_DELETE(*iter);
	//}
	for (auto iter = barricades.begin(); iter != barricades.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	barricades.clear();
}

void PathFinder::Render()
{
	for (auto iter = path.begin(); iter != path.end(); ++iter)
	{
		PathNode* pathNode = *iter;
		if (pathNode)
		{
			pathNode->Render();
		}
	}

	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	GameManager::GetDevice()->SetFVF(FVF_PositionColor::FVF);
	GameManager::GetDevice()->DrawPrimitiveUP(
		D3DPT_LINELIST,
		lineCount,
		&edgeLine[0],
		sizeof(FVF_PositionColor));
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);

	for (auto iter = barricades.begin(); iter != barricades.end(); ++iter)
	{
		Barricade* bNode = *iter;
		if (bNode)
		{
			bNode->Render();
		}
	}
}

bool PathFinder::CheckBarricadeCollision(const D3DXVECTOR3& start, const D3DXVECTOR3& end)
{
	bool collision = false;

	Ray ray;
	ray.origin = start;
	ray.direction = end - start;
	float startToEndLength = D3DXVec3LengthSq(&ray.direction);
	D3DXVec3Normalize(&ray.direction, &ray.direction);

	for (int i = 0; i < BARRICADE_COUNT; ++i)
	{
		Barricade* barricade = barricades[i];
		bool isPick = ray.IsPicked(barricade->GetBoundingSphere());
		if (isPick)
		{
			D3DXVECTOR3 startToBarricade = barricade->GetPosition() - start;
			float startToBarricadeLength = D3DXVec3LengthSq(&startToBarricade);
			if (startToBarricadeLength < startToEndLength)
			{
				collision = true;
				break;
			}
			else
			{
				D3DXVECTOR3 endToBarricade = barricade->GetPosition() - end;
				float endToBarricadeLength = D3DXVec3LengthSq(&endToBarricade);
				if (endToBarricadeLength < barricade->GetRadius() * barricade->GetRadius())
				{
					collision = true;
					break;
				}
			}
		}
	}

	return collision;
}

int PathFinder::FindNearestPathNode(const D3DXVECTOR3& pos)
{
	int nearestNodeIndex = -1;
	float nearestDistance = INFINITY_DISTANCE;

	for (int i = 0; i < PATH_NODE_COUNT; ++i)
	{
		PathNode* node = path[i];
		const D3DXVECTOR3& nodePosition = node->GetPosition();

		bool isCollision = CheckBarricadeCollision(pos, nodePosition);
		if (isCollision == false)
		{
			D3DXVECTOR3 v = nodePosition - pos;
			float distance = D3DXVec3LengthSq(&v);
			if (distance < nearestDistance)
			{
				nearestDistance = distance;
				nearestNodeIndex = i;
			}
		}
	}

	return nearestNodeIndex;
}

void PathFinder::PathOptimize(std::list<int>& pathList, const D3DXVECTOR3& start, const D3DXVECTOR3& end)
{
	int startIndex = pathList.front();
	for (auto iter = ++pathList.cbegin(); iter != pathList.cend(); ++iter)
	{
		bool isCollision = CheckBarricadeCollision(start, path[*iter]->GetPosition());
		if (isCollision == false)
		{
			startIndex = *iter;
		}
		else
		{
			break;
		}
	}
	while (pathList.front() != startIndex)
	{
		pathList.pop_front();
	}

	int endIndex = pathList.back();
	for (auto iter = pathList.crbegin(); iter != pathList.crend(); ++iter)
	{
		bool isCollision = CheckBarricadeCollision(end, path[*iter]->GetPosition());
		if (isCollision == false)
		{
			endIndex = *iter;
		}
		else
		{
			break;
		}
	}
	while (pathList.back() != endIndex)
	{
		pathList.pop_back();
	}
}
