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

		// vbBatchSize������ �ϳ��� �׸�
		// offset�� ũ�Ⱑ ���ؽ������� ���� ���������� �ٽ� ó������
		if (vbOffset >= vbSize)
		{
			vbOffset = 0;
		}

		Particle* v = 0;

		vertexBuffer->Lock(vbOffset * sizeof(Particle),	vbBatchSize * sizeof(Particle), (void**)&v,	vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);	//offset�� 0�̸� D3DLOCK_DISCARD�� �� ����� ó������ ����, �ƴϸ� D3DLOCK_NOOVERWRITE�� �������� �״�� �ΰ� �ڿ� �����̱�

		DWORD numParticlesInBatch = 0;

		// ��� ��ƼŬ�� �� �׸���.
		for (auto iter = particles.begin(); iter != particles.end(); ++iter)
		{
			if (iter->isAlive)
			{
				//���ؽ� ���۷� ����
				v->position = iter->position;
				v->color = (D3DCOLOR)iter->color;
				v++; //���ؽ� ������ ���� ��ġ�� �̵�;

				numParticlesInBatch++; //���� ���� ����

				// �� �ܰ�(batch, ���)�� �� ������ Ȯ��
				if (numParticlesInBatch == vbBatchSize)
				{
					// �� �ܰ� ���ΰ��� �׸���
					vertexBuffer->Unlock();

					GameManager::GetDevice()->DrawPrimitive(D3DPT_POINTLIST, vbOffset, vbBatchSize);

					// �׸��� ���� ���� �ܰ踦 ä���					
					vbOffset += vbBatchSize;	// ���� �ܰ�� �̵�

					// offset�� ũ�Ⱑ ���ؽ������� ���� ���������� �ٽ� ó������
					if (vbOffset >= vbSize)
						vbOffset = 0;

					vertexBuffer->Lock(
						vbOffset    * sizeof(Particle),
						vbBatchSize * sizeof(Particle),
						(void**)&v,
						vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);	//ó�� ���ۺκа� ����

					numParticlesInBatch = 0; // �ٽ� ó������ ī����
				}
			}
		}
		vertexBuffer->Unlock();	//������ ���

		// ������ �׸���
		if (numParticlesInBatch)
		{
			GameManager::GetDevice()->DrawPrimitive(D3DPT_POINTLIST, vbOffset, numParticlesInBatch);
		}

		// ���� �������� ���� offset����
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

	// �ؽ����� ���� ���
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