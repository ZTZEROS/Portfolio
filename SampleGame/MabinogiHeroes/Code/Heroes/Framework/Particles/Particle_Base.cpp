#include "stdafx.h"
#include "Particle_Base.h"


Particle_Base::Particle_Base()
{
}


Particle_Base::~Particle_Base()
{
}

bool Particle_Base::Initialize(char* textureFileName)
{
	HRESULT hr = 0;
	hr = GameManager::GetDevice()->CreateVertexBuffer(
		vbSize * sizeof(Particle),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		Particle::FVF,
		D3DPOOL_DEFAULT,
		&vertexBuffer,
		0);
	if (FAILED(hr))
	{
		MessageBoxA(0, "CreateVertexBuffer() - FAILED", "Error", MB_OK);
		return false;
	}
	texture = TextureManager::GetTexture(textureFileName);
	D3DXMatrixIdentity(&world);

	return true;
}

void Particle_Base::Destroy()
{
	texture = nullptr;
	SAFE_RELEASE(vertexBuffer);
	particles.clear();

}

void Particle_Base::Render()
{
	if (!particles.empty())
	{
		PreRender();

		GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
		GameManager::GetDevice()->SetTexture(0, texture);
		GameManager::GetDevice()->SetFVF(Particle::FVF);
		GameManager::GetDevice()->SetStreamSource(0, vertexBuffer, 0, sizeof(Particle));

		// vbBatchSize단위로 하나씩 그림
		// offset의 크기가 버텍스버퍼의 끝에 도달했으면 다시 처음부터
		if (vbOffset >= vbSize)
		{
			vbOffset = 0;
		}

		Particle* v = 0;

		vertexBuffer->Lock(vbOffset * sizeof(Particle),	vbBatchSize * sizeof(Particle), (void**)&v,	vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);	//offset이 0이면 D3DLOCK_DISCARD로 다 지우고 처음부터 쓰기, 아니면 D3DLOCK_NOOVERWRITE로 이전것은 그대로 두고 뒤에 덧붙이기

		DWORD numParticlesInBatch = 0;

		// 모든 파티클을 다 그린다.
		for (auto iter = particles.begin(); iter != particles.end(); ++iter)
		{
			if (iter->isAlive)
			{
				//버텍스 버퍼로 복사
				v->position = iter->position;
				v->color = (D3DCOLOR)iter->color;
				v++; //버텍스 버퍼의 다음 위치로 이동;

				numParticlesInBatch++; //복사 갯수 증가

				// 한 단계(batch, 덩어리)가 다 들어갔는지 확인
				if (numParticlesInBatch == vbBatchSize)
				{
					// 한 단계 모인것을 그리기
					vertexBuffer->Unlock();

					GameManager::GetDevice()->DrawPrimitive(D3DPT_POINTLIST, vbOffset, vbBatchSize);

					// 그리는 동안 다음 단계를 채우기					
					vbOffset += vbBatchSize;	// 다음 단계로 이동

					// offset의 크기가 버텍스버퍼의 끝에 도달했으면 다시 처음부터
					if (vbOffset >= vbSize)
						vbOffset = 0;

					vertexBuffer->Lock(
						vbOffset    * sizeof(Particle),
						vbBatchSize * sizeof(Particle),
						(void**)&v,
						vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);	//처음 시작부분과 동일

					numParticlesInBatch = 0; // 다시 처음부터 카운팅
				}
			}
		}
		vertexBuffer->Unlock();	//마지막 잠금

		// 남은것 그리기
		if (numParticlesInBatch)
		{
			GameManager::GetDevice()->DrawPrimitive(D3DPT_POINTLIST, vbOffset, numParticlesInBatch);
		}

		// 다음 프레임을 위해 offset증가
		vbOffset += vbBatchSize;

		PostRender();
	}
}

void Particle_Base::AddParticle()
{
	ParticleAttribute attribute;
	ResetParticle(attribute);
	particles.push_back(attribute);
}

void Particle_Base::Reset()
{
	for (auto iter = particles.begin(); iter != particles.end(); ++iter)
	{
		ResetParticle(*iter);
	}
}

bool Particle_Base::IsEmpty()
{
	return particles.empty();
}

bool Particle_Base::IsDead()
{
	for (auto iter = particles.begin(); iter != particles.end(); ++iter)
	{
		if (iter->isAlive)
			return false;
	}
	return true;
}

void Particle_Base::PreRender()
{
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	GameManager::GetDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	GameManager::GetDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	GameManager::GetDevice()->SetRenderState(D3DRS_POINTSIZE, FloatToDWORD(size));
	GameManager::GetDevice()->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDWORD(0.0f));

	// control the size of the particle relative to distance
	GameManager::GetDevice()->SetRenderState(D3DRS_POINTSCALE_A, FloatToDWORD(0.0f));
	GameManager::GetDevice()->SetRenderState(D3DRS_POINTSCALE_B, FloatToDWORD(0.0f));
	GameManager::GetDevice()->SetRenderState(D3DRS_POINTSCALE_C, FloatToDWORD(1.0f));

	// 텍스쳐의 알파 사용
	GameManager::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	GameManager::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	GameManager::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	GameManager::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GameManager::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void Particle_Base::PostRender()
{
	GameManager::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	GameManager::GetDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	GameManager::GetDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
}