#include "stdafx.h"
#include "Character_PathMove.h"


Character_PathMove::Character_PathMove()
{
}


Character_PathMove::~Character_PathMove()
{
}

LRESULT Character_PathMove::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (message)
	{
		case WM_LBUTTONDOWN:
		{
			if (grid&&pathFinder)
			{
				POINT mousePoint;
				mousePoint.x = GET_X_LPARAM(lParam);
				mousePoint.y = GET_Y_LPARAM(lParam);
				Ray ray = Ray::CalcRayFromSceenPoint(mousePoint);
				const std::vector<D3DXVECTOR3>& groundVertex = grid->GetGroundVertex();

				for (size_t i = 0; i < groundVertex.size(); i += 3)
				{
					float u, v, distance;
					if (D3DXIntersectTri(&groundVertex[i], &groundVertex[i + 1], &groundVertex[i + 2],
						&ray.origin, &ray.direction, &u, &v, &distance))
					{
						D3DXVECTOR3 targetPosition = ray.origin + ray.direction * distance;
						/*D3DXVECTOR3 targetPosition = groundVertex[i]
						+ (groundVertex[i + 1] - groundVertex[i]) * u
						+ (groundVertex[i + 2] - groundVertex[i]) * v;*/

						bool isCollision = pathFinder->CheckBarricadeCollision(position, targetPosition);
						if (isCollision)
						{
							//길찾기
							int startIndex = pathFinder->FindNearestPathNode(position);
							int endIndex = pathFinder->FindNearestPathNode(targetPosition);

							if (startIndex != -1 && endIndex != -1)
							{
								std::list<int> path;
								pathFinder->PathFind(path, startIndex, endIndex);
								pathFinder->PathOptimize(path, position, targetPosition);
								
								//액션 추가
								SAFE_DELETE(action);

								ActionSequence* sequence = new ActionSequence;
								auto iterStart = path.cbegin();
								auto iterNext = ++path.cbegin();
								float length = 1.0f;
								D3DXVECTOR3 temp;

								//출발점 -> 첫번째 노드
								ActionMove* firstMove = new ActionMove;
								const D3DXVECTOR3& startPos = pathFinder->GetPathNodePosition(*iterStart);
								temp = startPos - position;
								length = D3DXVec3Length(&temp);
								firstMove->SetStart(position);
								firstMove->SetGoal(startPos);
								firstMove->SetDuration(0.1f * length);
								firstMove->SetTarget(this);
								firstMove->SetDelegate(sequence);
								sequence->AddAction(firstMove);

								//첫번째 노드 -> 마지막 노드
								while (iterStart != path.cend() && iterNext != path.cend())
								{
									ActionMove* move = new ActionMove;

									const D3DXVECTOR3& start = pathFinder->GetPathNodePosition(*iterStart);
									const D3DXVECTOR3& next = pathFinder->GetPathNodePosition(*iterNext);
									temp = next - start;
									length = D3DXVec3Length(&temp);
									move->SetStart(start);
									move->SetGoal(next);
									move->SetDuration(0.1f * length);
									move->SetTarget(this);
									move->SetDelegate(sequence);
									sequence->AddAction(move);

									++iterStart;
									++iterNext;
								}

								//마지막 노드 -> 도착점
								ActionMove* LastMove = new ActionMove;
								const D3DXVECTOR3& lastPos = pathFinder->GetPathNodePosition(*iterStart);
								temp = targetPosition - lastPos;
								length = D3DXVec3Length(&temp);
								LastMove->SetStart(lastPos);
								LastMove->SetGoal(targetPosition);
								LastMove->SetDuration(0.1f * length);
								LastMove->SetTarget(this);
								LastMove->SetDelegate(sequence);
								sequence->AddAction(LastMove);

								//세팅 및 시작
								action = sequence;
								action->Start();
							}
						}
						else
						{
							//기존 이동
							D3DXVECTOR3 targetLength = targetPosition - position;
							float length = D3DXVec3Length(&targetLength);

							SAFE_DELETE(action);
							ActionMove* move = new ActionMove;
							move->SetStart(position);
							move->SetGoal(targetPosition);
							move->SetDuration(length * 0.1f);
							move->SetTarget(this);
							move->Start();
							SetAction(move);
						}

						
					}
				}
			}

		}
		break;
	}
	return result;
}
