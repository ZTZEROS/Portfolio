#include "stdafx.h"
#include "AseFrame.h"

AseFrame::AseFrame()
{
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&local);
}

AseFrame::~AseFrame()
{
	SAFE_RELEASE(materialTexture);
}

void AseFrame::Update(int keyFrame, D3DXMATRIXA16* parent)
{
	D3DXMATRIXA16 rotation, translation;
	GetLocalRotate(keyFrame, rotation);
	GetLocalTranslate(keyFrame, translation);
	local = rotation * translation;
	world = local;
	if (parent)
	{
		world *= (*parent);
	}

	for (auto iter = childArray.cbegin(); iter != childArray.cend(); ++iter)
	{
		(*iter)->Update(keyFrame, &world);
	}
}

void AseFrame::Render()
{
	if (materialTexture)
	{
		GameManager::GetDevice()->SetMaterial(&materialTexture->material);
		if (textureOn == true)
		{
			GameManager::GetDevice()->SetTexture(0, materialTexture->texture);

		}
		else
		{
			D3DMATERIAL9 mat;
			ZeroMemory(&mat, sizeof(D3DMATERIAL9));
			mat.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
			mat.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
			mat.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
			GameManager::GetDevice()->SetMaterial(&mat);
			GameManager::GetDevice()->SetTexture(0, nullptr);
		}
		GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
		GameManager::GetDevice()->SetFVF(FVF_PositionNormalTexture::FVF);
		GameManager::GetDevice()->DrawPrimitiveUP(
			D3DPT_TRIANGLELIST,
			vertexArray.size() / 3,
			&vertexArray[0],
			sizeof(FVF_PositionNormalTexture));
	}

	for (auto iter = childArray.cbegin(); iter != childArray.cend(); ++iter)
	{
		(*iter)->Render();
	}
}

void AseFrame::Destroy()
{
	for (auto iter = childArray.begin(); iter != childArray.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
}

void AseFrame::AddChild(AseFrame* child)
{
	if (child != nullptr)
	{
		childArray.push_back(child);
	}
}

void AseFrame::CalcOrigLocalTransform(D3DXMATRIXA16* parent)
{
	local = world;
	if (parent)
	{
		D3DXMATRIXA16 matInvParent;
		D3DXMatrixInverse(&matInvParent, nullptr, parent);
		local = world * matInvParent;
	}

	for (auto iter = childArray.cbegin(); iter != childArray.cend(); ++iter)
	{
		(*iter)->CalcOrigLocalTransform(&world);
	}
}

void AseFrame::GetLocalTranslate(IN int keyFrame, OUT D3DXMATRIXA16& matrix)
{
	//float t = (keyFrame - 640) / (1280 - 640);	//keyFrame = 1000
	//	//interpolation;
	//	D3DXVec3Lerp(&보간된위치, &시작위치, &도착위치, 시간(0~1사이));

	D3DXMatrixIdentity(&matrix);
	if (posTrack.empty())
	{
		matrix._41 = local._41;
		matrix._42 = local._42;
		matrix._43 = local._43;
	}
	else if (keyFrame <= posTrack.front().frame)
	{
		matrix._41 = posTrack.front().pos.x;
		matrix._42 = posTrack.front().pos.y;
		matrix._43 = posTrack.front().pos.z;
	}
	else if (keyFrame >= posTrack.back().frame)
	{
		matrix._41 = posTrack.back().pos.x;
		matrix._42 = posTrack.back().pos.y;
		matrix._43 = posTrack.back().pos.z;
	}
	else
	{
		int next = 0;
		for (size_t i = 0; i < posTrack.size(); ++i)
		{
			if (keyFrame < posTrack[i].frame)
			{
				next = i;
				break;
			}
		}
		int prev = next - 1;
		float t = (float)(keyFrame - posTrack[prev].frame) / (float)(posTrack[next].frame - posTrack[prev].frame);
		D3DXVECTOR3 v;
		D3DXVec3Lerp(&v, &posTrack[prev].pos, &posTrack[next].pos, t);
		matrix._41 = v.x;
		matrix._42 = v.y;
		matrix._43 = v.z;

	}
}

void AseFrame::GetLocalRotate(IN int keyFrame, OUT D3DXMATRIXA16& matrix)
{
	//D3DXMatrixRotationQuaternion(&출력용메트릭스, &입력용쿼터니언);

	//D3DXQuaternionSlerp(&출력용 쿼터니언, &시작 쿼터니언, &도착 쿼터니언, 시간);

	D3DXMatrixIdentity(&matrix);
	if (rotTrack.empty())
	{
		matrix = local;
		matrix._41 = 0;
		matrix._42 = 0;
		matrix._43 = 0;
	}
	else if (keyFrame <= rotTrack.front().frame)
	{
		D3DXMatrixRotationQuaternion(&matrix, &rotTrack.front().quaternion);
	}
	else if (keyFrame >= rotTrack.back().frame)
	{
		D3DXMatrixRotationQuaternion(&matrix, &rotTrack.back().quaternion);
	}
	else
	{
		int nNext = 0;
		for (size_t i = 0; i < rotTrack.size(); ++i)
		{
			if (keyFrame < rotTrack[i].frame)
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;
		float t = (keyFrame - rotTrack[nPrev].frame) / (float)(rotTrack[nNext].frame - rotTrack[nPrev].frame);

		D3DXQUATERNION q;
		D3DXQuaternionSlerp(&q, &rotTrack[nPrev].quaternion, &rotTrack[nNext].quaternion, t);		
		D3DXMatrixRotationQuaternion(&matrix, &q);
	}
}

void AseFrame::SetWorldTransform(D3DXMATRIXA16* matrix)
{
	if (matrix == nullptr)
		return;
	world = *matrix;
}

void AseFrame::SetTextureOn(bool on)
{
	textureOn = on;

	for (auto iter = childArray.begin(); iter != childArray.end(); ++iter)
	{
		(*iter)->SetTextureOn(on);
	}
}